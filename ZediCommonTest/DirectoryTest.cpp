/** @file DirectoryTest.cpp
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

class DirectoryTest: public testing::Test {
public:
    virtual void TearDown() {
        ::system("rm -rf ./DirectoryTest 2> /dev/null");
        ::system("rm -rf /tmp/DirectoryTest 2> /dev/null");
    }
};

TEST_F(DirectoryTest, shouldCreateAbsoluteDirectoryPath) {
    Directory dir("/tmp/DirectoryTest");
    ASSERT_FALSE(dir.exists());
    dir.mkdirs();
    ASSERT_TRUE(dir.exists());
}

TEST_F(DirectoryTest, shouldCreateRelativeDirectoryPath) {
    Directory dir("DirectoryTest");
    ASSERT_FALSE(dir.exists());
    dir.mkdirs();
    ASSERT_TRUE(dir.exists());
}

TEST_F(DirectoryTest, shouldNotDeleteDirectoryIfNotEmpty) {
    Directory dir("/tmp/DirectoryTest");
    Directory subDir("/tmp/DirectoryTest/Foo");
    subDir.mkdirs();
    ASSERT_TRUE(dir.exists());
    ASSERT_FALSE(dir.turf());
    ASSERT_TRUE(dir.exists());
}

TEST_F(DirectoryTest, shouldDeleteDirectoryIfForcedAndNotEmpty) {
    Directory dir("/tmp/DirectoryTest");
    Directory subDir("/tmp/DirectoryTest/Foo");
    subDir.mkdirs();
    ASSERT_TRUE(dir.exists());
    ASSERT_TRUE(dir.turf(true));
    ASSERT_FALSE(dir.exists());
}

}
}
