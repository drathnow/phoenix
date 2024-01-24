/** @file FileTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 21, 2014 by daver:
 * BugFix_ Apr 03, 2014 by eb:	found by tester script when tranformed: removed multiple mkdir cmd
 * IncrDev Oct 22, 2014 by eb:  add in test for File::mkdirs
 * @endhistory
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <iostream>
#include <gtest/gtest.h>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
#include <memory>
#include <fstream>

#include <File.h>

namespace zios {
namespace common_tests {

using namespace zios::common;
using namespace std;

class FileTest: public testing::Test {
public:
    virtual void SetUp() {
        system("mkdir ./FileTest");
    }

    virtual void TearDown() {
        system("rm -rf ./FileTest");
    }
};

int filter(const struct dirent* dir) {
    int number;
    return ::sscanf(dir->d_name, "foo%d.dat", &number) == 1;
}

int myVersionSort(const struct dirent **dir1, const struct dirent **dir2) {
    return ::strverscmp((*dir2)->d_name, (*dir1)->d_name);
}

TEST_F(FileTest, shouldAssignFile) {
    File fromfile("FileTest/FileTest2","foo.dat");
    File toFile;
    toFile = fromfile;
    ASSERT_STREQ(fromfile.absolutePath(), toFile.absolutePath());
}

TEST_F(FileTest, shouldCopyFile) {
    File fromfile("FileTest/FileTest2","foo.dat");
    File toFile(fromfile);
    toFile = fromfile;
    ASSERT_STREQ(fromfile.absolutePath(), toFile.absolutePath());
}

TEST_F(FileTest, shouldDeleteFile) {
    ::system("touch ./FileTest/foo.dat");
    File f("./FileTest", "foo.dat");
    ASSERT_TRUE(f.exists());
    f.turf();
    ASSERT_FALSE(f.exists());
}

TEST_F(FileTest, shouldCreateDirectoryTree) {
    system("rm -rf ./FileTest");        // make sure directory is gone.
    File file("FileTest/FileTest2","foo2.dat");
    EXPECT_TRUE(file.mkdirs());
    // check that there is a FileTest Dir and FileTest2 subdir:
    struct stat sb;
    ASSERT_NE(-1, stat("./FileTest", &sb));
    EXPECT_TRUE(S_ISDIR(sb.st_mode));
    ASSERT_NE(-1, stat("./FileTest/FileTest2", &sb));
    EXPECT_TRUE(S_ISDIR(sb.st_mode));


    // go check for # directory entries for each dir:
    // remember we need to ignore  "." and ".."
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir("FileTest"); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if ((strcmp(".",entry->d_name) != 0) && (strcmp("..",entry->d_name) != 0))
             file_count++;
    }
    closedir(dirp);
    EXPECT_EQ(1,file_count);
    file_count = 0;
    dirp = opendir("FileTest/FileTest2"); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if ((strcmp(".",entry->d_name) != 0) && (strcmp("..",entry->d_name) != 0))
            file_count++;
    }
    closedir(dirp);
    EXPECT_EQ(0, file_count);

}

TEST_F(FileTest, shouldBuildWithParent) {
    const char* expected = "/home/daver/cpp/zios/TestReporting/TrackingContainerQueueIntgTest/msg0000000001.que";
    const char* fqn = "/home/daver/cpp/zios/TestReporting/TrackingContainerQueueIntgTest";
    const char* name = "msg0000000001.que";
    File file(fqn, name);

    ASSERT_STREQ(expected, file.absolutePath());
}

TEST_F(FileTest, shouldDoNameCorrectly) {
    const char *fqn = "/home/daver/cpp/zios/TestReporting/./TrackingContainerQueueIntgTest/msg0000000001.que";
    File file(fqn);

    ASSERT_STREQ(fqn, (const char*)file.absolutePath());
    ASSERT_STREQ("msg0000000001.que", (const char*)file.name());
}

TEST_F(FileTest, shouldReturnParent) {
    std::ostringstream oss;
    char* cwd = get_current_dir_name();
    oss << cwd << "/" << "foo.dat";
    File file("foo.dat");
    ASSERT_STREQ(cwd, (char*)file.parent().c_str());
    ::free(cwd);
}

TEST_F(FileTest, shouldReturnAbsoluteName) {
    std::ostringstream oss;
    char* cwd = get_current_dir_name();
    oss << cwd << "/" << "foo.dat";
    File file("foo.dat");
    ASSERT_STREQ(oss.str().c_str(), (char*)file.absolutePath());
    ::free(cwd);
}

TEST_F(FileTest, shouldReturnName) {
    std::ostringstream oss;
    File file("foo.dat");
    ASSERT_STREQ("foo.dat", (char*)file.name());
}

TEST_F(FileTest, shouldReturnFilteredFileList) {
    system("touch ./FileTest/foo1.dat");
    system("touch ./FileTest/foo2.dat");
    std::vector<std::string> result;

    File file("./FileTest");
    file.list(result, filter, myVersionSort);

    ASSERT_EQ((uint32_t)2, result.size());
    ASSERT_EQ(0, ::strcmp("foo1.dat", result[0].c_str()));
    ASSERT_EQ(0, ::strcmp("foo2.dat", result[1].c_str()));
}

TEST_F(FileTest, shouldReturnFileList) {
    system("touch ./FileTest/foo1.dat");
    system("touch ./FileTest/foo2.dat");
    std::vector<std::string> result;

    File file("./FileTest");
    file.list(result);

    ASSERT_EQ((uint32_t)4, result.size());
    for (std::vector<std::string>::iterator iter = result.begin();
            iter != result.end(); ) {
        if (*iter == "foo1.dat" || *iter == "foo2.dat")
            result.erase(iter);
        else
            ++iter;
    }
    ASSERT_EQ((uint32_t)2, result.size());
}


TEST_F(FileTest, shouldCreateTempFilename) {
    File file = File::createTempFile("foo", ".bar");
    std::string name((char*)file.name());
    ASSERT_EQ((uint32_t)0, name.find_first_of("/tmp/foo"));
    ASSERT_TRUE(name.length() > 10);
    uint32_t expPos = name.length()- 4;
    ASSERT_EQ(expPos, name.find_first_of(".bar"));
}

TEST_F(FileTest, shouldVerifyFileExists) {
    system("touch ./FileTest/Foo.dat");
    File file("./FileTest/Foo.dat");
    file.deleteOnExit();
    ASSERT_TRUE(file.exists());
}

TEST_F(FileTest, shouldVerifyFileDoesNotExists) {
    File file("FooManChoo.spooge");
    ASSERT_FALSE(file.exists());
}

TEST_F(FileTest, shouldReturnSize) {
    system("touch ./FileTest/foo.dat");
    fstream stream("./FileTest/foo.dat", ios_base::app);
    stream << "Hello World";
    stream.close();

    struct stat sb;
    ASSERT_NE(-1, stat("./FileTest/foo.dat", &sb));
    uint64_t expectedSize = sb.st_size;
    File file("./FileTest/foo.dat");
    ASSERT_EQ(expectedSize, file.size());
}

TEST_F(FileTest, shouldVerifyFileExecutable) {
    system("touch ./FileTest/Foo.ex");
    system("chmod u+x ./FileTest/Foo.ex");
    File file("./FileTest/Foo.ex");
    file.deleteOnExit();
    ASSERT_TRUE(file.isExecutable());
}

TEST_F(FileTest, shouldVerifyFileNotExecutable) {
    system("touch ./FileTest/Foo.nex");
    File file("./FileTest/Foo.nex");
    file.deleteOnExit();
    ASSERT_FALSE(file.isExecutable());
}

}
}
