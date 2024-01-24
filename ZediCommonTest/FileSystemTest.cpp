/** @file FileSystemTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 19, 2014 by daver:
 * @endhistory
 */

#include <gtest/gtest.h>
#include <File.h>
#include <unistd.h>
#include <memory>
#include <sstream>

namespace zios {
namespace common_tests {

using namespace std;
using namespace zios::common;

class FileSystemTest : public testing::Test {
public:

    void SetUp() {
        system("rm -rf ./foo");
    }

    void TearDown() {
        system("rm -rf ./foo");
    }
};

TEST_F(FileSystemTest, shouldCreateDirectoryTree) {
    File dir("./foo/bar/spooge");
    FileSystem fileSystem;
    ASSERT_FALSE(dir.exists());
    fileSystem.createDirectories(dir.absolutePath());
    ASSERT_TRUE(dir.exists());

}

TEST_F(FileSystemTest, shouldCreateDirectory) {
    File dir("./foo");
    FileSystem fileSystem;
    ASSERT_FALSE(dir.exists());
    fileSystem.createDirectory(dir.absolutePath());
    ASSERT_TRUE(dir.exists());
}

TEST_F(FileSystemTest, shouldCreateTempFilename) {
    FileSystem fileSystem;

    string result = FileSystem::createTempFilename((const uint8_t*)"foo", (const uint8_t*)".dat", NULL);
    ASSERT_EQ((size_t)0, result.find("/tmp/foo"));
}

TEST_F(FileSystemTest, shouldNormalizePathWithNothing) {
    FileSystem fileSystem;
    char* cwd = get_current_dir_name();
    ostringstream oss;
    string normalPath;
    string path;

    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(cwd, normalPath.c_str());
    ::free(cwd);
}

TEST_F(FileSystemTest, shouldNormalizePathWithFullPath1) {
    FileSystem fileSystem;
    ostringstream oss;
    string normalPath;
    string path("/bar/foo.dat");

    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(path.c_str(), normalPath.c_str());
}

TEST_F(FileSystemTest, shouldReplaceOnlyTildeWithHomeDir) {
    const char* homedir = FileSystem::homeDir();
    FileSystem fileSystem;
    ostringstream oss;
    string normalPath;
    string path("~");

    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(homedir, normalPath.c_str());
}

TEST_F(FileSystemTest, shouldReplaceTildeWithHomeDir) {
    string expectedPath(FileSystem::homeDir());
    expectedPath.append("/bar/foo.dat");
    FileSystem fileSystem;
    ostringstream oss;
    string normalPath;
    string path("~/bar/foo.dat");

    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(expectedPath.c_str(), normalPath.c_str());
}

TEST_F(FileSystemTest, shouldNormalizePathWithFullPath2) {
    FileSystem fileSystem;
    ostringstream oss;
    string normalPath;
    string path("/foo.dat");

    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(path.c_str(), normalPath.c_str());
}

TEST_F(FileSystemTest, shouldNormalizePathWithParentDir) {
    FileSystem fileSystem;
    char* cwd = get_current_dir_name();
    string curDir(cwd);
    string parentDir = curDir.substr((size_t)0, curDir.find_last_of('/'));
    ostringstream oss;
    string normalPath;
    string path("../foo.dat");
    oss << cwd << "/" << path;
    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(oss.str().c_str(), normalPath.c_str());
    ::free(cwd);
}

TEST_F(FileSystemTest, shouldNormalizePathWithCurDir) {
    FileSystem fileSystem;
    char* cwd = get_current_dir_name();
    ostringstream oss;
    string normalPath;
    string path("./foo.dat");
    oss << cwd << "/" << path;
    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(oss.str().c_str(), normalPath.c_str());
    ::free(cwd);
}

TEST_F(FileSystemTest, shouldNormalizePathWithFilenameOnly) {
    FileSystem fileSystem;
    char* cwd = get_current_dir_name();
    ostringstream oss;
    string normalPath;
    string path("foo.dat");
    oss << cwd << "/" << path;
    fileSystem.normalizedPath(path, normalPath);
    ASSERT_STREQ(oss.str().c_str(), normalPath.c_str());
    ::free(cwd);
}


}
}
