#include <cstdint>
#include <utility>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MockByteBuffer.h>
#include <BlobValue.h>
#include <File.h>
#include <regex.h>
#include <vector>
#include <fstream>
#include <ByteBufferWrapper.h>
#include <ReadOnlyByteBuffer.h>
#include <iostream>
#include <HexDumper.h>

namespace zios::domain
{

using namespace std;
using namespace zios::common;

#define BlobValueTestSuite(test_suite_name, test_name)    \
  GTEST_TEST_(test_suite_name, test_name, BlobValueTest,  \
              ::testing::internal::GetTestTypeId())

static const char LT_TEST_BLOB[] = "This is a test blob";
static char LE_TEST_BLOB[BlobValue::MAX_MEMORY_SIZE];
static char GT_TEST_BLOB[BlobValue::MAX_MEMORY_SIZE+1];

static int blobFilter(const struct dirent *de)
{
    regex_t regex;
    const char *pattern = ".*blob$";
    regmatch_t matches[1];
    ::regcomp(&regex, pattern, REG_EXTENDED);
    return ::regexec(&regex, de->d_name, 1, matches, 0) == 0;
}

static void clearAllBlobFilesFromTemp()
{
    vector<string> files;
    const char *tmpPath = FileSystem::tempDirectoryPath();
    Directory tmpDir(tmpPath);
    tmpDir.list(files, &blobFilter);
    for (vector<string>::const_iterator iter = files.begin(); iter != files.end(); iter++)
    {
        File deadFile(tmpPath, (*iter).c_str());
        deadFile.turf();
    }
}

static vector<string> blobFilenamesFromTemp()
{
    vector<string> files;
    const char *tmpPath = FileSystem::tempDirectoryPath();
    Directory tmpDir(tmpPath);
    tmpDir.list(files, &blobFilter);
    return files;
}

class BlobValueTest: public testing::Test
{
public:
    BlobValueTest() = default;
    virtual ~BlobValueTest() = default;


    void SetUp()
    {
        clearAllBlobFilesFromTemp();
    }

    void TearDown()
    {
        clearAllBlobFilesFromTemp();
    }

    const uint8_t* valueFromBlobValue(BlobValue& blobValue)
    {
        return blobValue._value;
    }

    HexDumper _hexDumper;
};

BlobValueTestSuite(WhenBlobValueIsConstructedWithBlobLessThanMaxMemory, shouldNotCreateFile)
{
    BlobValue blobValue((const uint8_t*)LT_TEST_BLOB, ::strlen(LT_TEST_BLOB));
    ASSERT_EQ(0, blobFilenamesFromTemp().size());
}

BlobValueTestSuite(WhenBlobValueIsConstructedWithBlobEqualToMaxMemory, shouldNotCreateFile)
{
    BlobValue blobValue((const uint8_t*)LE_TEST_BLOB, sizeof(LE_TEST_BLOB));
    ASSERT_EQ(0, blobFilenamesFromTemp().size());
}

BlobValueTestSuite(WhenBlobValueIsConstructedWithBlobGreaterThanMaxMemory, shouldCreateFile)
{
    uint32_t i = 0;
    for (i = 0; i < sizeof(GT_TEST_BLOB); i++)
    {
        GT_TEST_BLOB[i] = i;
    }

    BlobValue blobValue((const uint8_t*)GT_TEST_BLOB, sizeof(GT_TEST_BLOB));
    vector<string> blobFiles = blobFilenamesFromTemp();
    ASSERT_EQ(1, blobFiles.size());

    const char *tmpPath = FileSystem::tempDirectoryPath();
    File blobFile(tmpPath, blobFiles[0].c_str());
    uint8_t foo[blobValue.size()];
    fstream fs;
    fs.open(blobFile.absolutePath(), ios_base::binary | ios_base::in);
    ASSERT_TRUE(fs.is_open());
    fs.read((char*)foo, sizeof(foo));
    for (i = 0; i < sizeof(GT_TEST_BLOB); i++)
    {
        ASSERT_EQ(i & 0xff, (uint8_t)GT_TEST_BLOB[i]);
    }
}

BlobValueTestSuite(WhenBlobValueIsAppended, shouldAppendToEndOfBlobAndUpdateSize)
{
    BlobValue blobValue((const uint8_t*)LE_TEST_BLOB, ::strlen(LE_TEST_BLOB));
    blobValue.append((const uint8_t*)"Hello World", sizeof("Hello World"));

    string expectedString(LE_TEST_BLOB);
    expectedString.append("Hello World");

    ASSERT_EQ(0, ::strncmp((char*)valueFromBlobValue(blobValue), expectedString.c_str(), expectedString.size()));
}

BlobValueTestSuite(WhenBlobValueIsAppendedAndBlobGoesGreaterThanMaxMemory, shouldDumpToFile)
{
    uint32_t expectedSize = ::strlen(LT_TEST_BLOB) + sizeof(GT_TEST_BLOB);
    uint32_t i = 0;
    for (i = 0; i < sizeof(GT_TEST_BLOB); i++)
    {
        GT_TEST_BLOB[i] = i;
    }

    BlobValue blobValue((const uint8_t*)LT_TEST_BLOB, ::strlen(LT_TEST_BLOB));
    blobValue.append((const uint8_t*)GT_TEST_BLOB, sizeof(GT_TEST_BLOB));

    ASSERT_EQ(expectedSize, blobValue.size());
    ASSERT_EQ(NULL, valueFromBlobValue(blobValue));

    vector<string> blobFiles = blobFilenamesFromTemp();
    ASSERT_EQ(1, blobFiles.size());

    const char *tmpPath = FileSystem::tempDirectoryPath();
    File blobFile(tmpPath, blobFiles[0].c_str());

    ASSERT_EQ(expectedSize, blobFile.size());

    uint8_t foo[expectedSize];
    fstream fs;
    fs.open(blobFile.absolutePath(), ios_base::binary | ios_base::in);
    ASSERT_TRUE(fs.is_open());
    fs.read((char*)foo, sizeof(foo));

    for (i = 0; i < ::strlen(LT_TEST_BLOB); i++)
    {
        ASSERT_EQ((uint8_t)LT_TEST_BLOB[i], foo[i]);
    }

    uint32_t j = 0;
    for (; i < expectedSize; i++)
    {
        ASSERT_EQ(j++ & 0xff, (uint8_t)foo[i]) << "j=" << (j-1) << ", i=" << i;
    }
}

BlobValueTestSuite(WhenBlobValueIsAskedToSerializeToByteBuffer, shouldSerializeToByteBuffer)
{
    BlobValue blobValue((const uint8_t*)LT_TEST_BLOB, ::strlen(LT_TEST_BLOB));
    char serializeBuffer[2048];
    ByteBufferWrapper byteBuffer((uint8_t*)serializeBuffer, sizeof(serializeBuffer));
    blobValue.serialize(byteBuffer);
    _hexDumper.dump((const char*)serializeBuffer, byteBuffer.position());
    byteBuffer.flip();

    ASSERT_EQ(::strlen(LT_TEST_BLOB), byteBuffer.getU32());
    ASSERT_EQ(0, ::strncmp(LT_TEST_BLOB, &serializeBuffer[4], ::strlen(LT_TEST_BLOB)));
}

}
