/** @file ConfigurationFileManagerTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 4, 2014 by daver:
 * @endhistory
 */



#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <log4cplus/configurator.h>
#include <Configuration.h>
#include <cstring>
#include <fstream>
#include <iostream>

namespace zios {
namespace common_tests {

using namespace std;
using namespace zios::common;


class ConfigurationFileManagerTest : public testing::Test {
public:

    void SetUp() {
        system("rm -rf ./ConfigurationFileManagerTest");
        system("mkdir ./ConfigurationFileManagerTest");
    }

    void TearDown() {
        system("rm -rf ./ConfigurationFileManagerTest");
    }
};

char charFromFile(File& file) {
    char readC;
    fstream fs(file.absolutePath(), ios_base::in);
    fs >> readC;
    fs.close();
    return readC;
}

TEST_F(ConfigurationFileManagerTest, shouldRenameCurrentFileToOneAndAllTheRestToPlusOne) {
    system("echo 0 > ./ConfigurationFileManagerTest/bar.dat");
    system("echo 1 > ./ConfigurationFileManagerTest/foo.dat");
    system("echo 2 > ./ConfigurationFileManagerTest/foo.dat.1");
    system("echo 3 > ./ConfigurationFileManagerTest/foo.dat.2");
    system("echo 4 > ./ConfigurationFileManagerTest/foo.dat.3");

    File expected("./ConfigurationFileManagerTest/foo.dat");
    File expected1("./ConfigurationFileManagerTest/foo.dat.1");
    File expected2("./ConfigurationFileManagerTest/foo.dat.2");
    File expected3("./ConfigurationFileManagerTest/foo.dat.3");
    File notExpected("./ConfigurationFileManagerTest/foo.dat.4");
    File newFile("./ConfigurationFileManagerTest/bar.dat");
    ConfigurationFileManager fileManager(expected, 3);

    File latest = fileManager.makeLatestConfigurationFile(newFile);

    ASSERT_STREQ("foo.dat", latest.name());
    ASSERT_FALSE(newFile.exists());
    ASSERT_TRUE(latest.exists());
    ASSERT_TRUE(expected.exists());
    ASSERT_TRUE(expected1.exists());
    ASSERT_TRUE(expected2.exists());
    ASSERT_TRUE(expected3.exists());
    ASSERT_FALSE(notExpected.exists());

    ASSERT_EQ('0', charFromFile(latest));
    ASSERT_EQ('1', charFromFile(expected1));
    ASSERT_EQ('2', charFromFile(expected2));
    ASSERT_EQ('3', charFromFile(expected3));
}

TEST_F(ConfigurationFileManagerTest, shouldRenameCurrentFileToOne) {
    system("echo 0 > ./ConfigurationFileManagerTest/bar.dat");
    system("echo 1 > ./ConfigurationFileManagerTest/foo.dat");

    File expected("./ConfigurationFileManagerTest/foo.dat");
    File expected1("./ConfigurationFileManagerTest/foo.dat.1");
    File newFile("./ConfigurationFileManagerTest/bar.dat");
    ConfigurationFileManager fileManager(expected, 3);

    File latest = fileManager.makeLatestConfigurationFile(newFile);

    ASSERT_STREQ("foo.dat", latest.name());
    ASSERT_FALSE(newFile.exists());
    ASSERT_TRUE(latest.exists());
    ASSERT_TRUE(expected1.exists());

    ASSERT_EQ('0', charFromFile(latest));
    ASSERT_EQ('1', charFromFile(expected1));
}

TEST_F(ConfigurationFileManagerTest, shouldRenameCurrentFile) {
    system("echo 0 > ./ConfigurationFileManagerTest/bar.dat");

    File newFile("./ConfigurationFileManagerTest/bar.dat");
    File file("./ConfigurationFileManagerTest/foo.dat");
    ConfigurationFileManager fileManager("./ConfigurationFileManagerTest/foo.dat", 3);

    File latest = fileManager.makeLatestConfigurationFile(newFile);

    ASSERT_STREQ("foo.dat", latest.name());
    ASSERT_FALSE(newFile.exists());
    ASSERT_TRUE(latest.exists());

    ASSERT_EQ('0', charFromFile(latest));
}

}
}
