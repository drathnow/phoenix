/** @file ZiosTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 23, 2017 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <zioscommon.h>
#include <string>

namespace zios {
namespace common_tests {

using namespace std;
using namespace zios::common;

static string homeDir() {
    string home;
    const char *homedir;
    homedir = getenv("HOME");
    home.append(homedir);
    return home;
}

TEST(WhenZiosFunctionIsAskedToReturnServerFilePath, shouldReturnFilename) {
    string expectedFilename(homeDir());
    expectedFilename.append(ZIOS_SERVER_FILE_PATH).append("/").append(ZIOS_SERVER_FILENAME);
    string filename;
    ASSERT_STREQ(expectedFilename.c_str(), zios_server_file_path(filename).c_str());
}

TEST(WhenZiosFunctionIsAskedToReturnServerStatePath, shouldReturnFilename) {
    string expectedFilename(homeDir());
    expectedFilename.append(ZIOS_SERVER_STATE_PATH).append("/").append(ZIOS_SERVER_STATE_FILENAME);
    string filename;
    ASSERT_STREQ(expectedFilename.c_str(), zios_state_file_path(filename).c_str());
}

}
}


