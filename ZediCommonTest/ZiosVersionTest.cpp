/** @file ZiosVersion.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 23, 2017 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <ZiosVersion.h>

namespace zios {
namespace common_tests {

using namespace std;
using namespace zios::common;

class WhenZiosVersionIsAskedToParseVersionString : public testing::Test {
public:
    ~WhenZiosVersionIsAskedToParseVersionString() {}
};

static const string GOOD_VERSION1A("1.2.3");
static const string GOOD_VERSION1B("3.  1.10");
static const string GOOD_VERSION1C("  409. 8 .888  ");
static const string GOOD_VERSION1D("1.2.3-DEV");
static const string GOOD_VERSION1E("1.2.3.897");
static const string GOOD_VERSION1F("1.2.3.897-DEV");
static const string GOOD_VERSION1G("1.2.3-DEV-LOCAL");

static const string BAD_VERSION1A("A.2.3");
static const string BAD_VERSION1B("1.B.3");
static const string BAD_VERSION1C("1.2.C");
static const string BAD_VERSION2A("-1.2.3");
static const string BAD_VERSION2B("1.-2.3");
static const string BAD_VERSION2C("1.2.-3");
static const string BAD_VERSION3A(".2.3");
static const string BAD_VERSION3B("1..3");
static const string BAD_VERSION3C("1.3.");
static const string BAD_VERSION3D("1.3.  ");

TEST_F(WhenZiosVersionIsAskedToParseVersionString, shouldParseStringAndPopulateValue) {
    ZiosVersion version;
    ASSERT_EQ(0, ZiosVersion::ziosVersionFromString(version, GOOD_VERSION1A));
    ASSERT_EQ(1, version.majorVersion());
    ASSERT_EQ(2, version.minorVersion());
    ASSERT_EQ(3, version.patchVersion());
    ASSERT_EQ(0, version.buildNumber());

    ASSERT_EQ(0, ZiosVersion::ziosVersionFromString(version, GOOD_VERSION1B));
    ASSERT_EQ(3, version.majorVersion());
    ASSERT_EQ(1, version.minorVersion());
    ASSERT_EQ(10, version.patchVersion());
    ASSERT_EQ(0, version.buildNumber());

    ASSERT_EQ(0, ZiosVersion::ziosVersionFromString(version, GOOD_VERSION1C));
    ASSERT_EQ(409, version.majorVersion());
    ASSERT_EQ(8, version.minorVersion());
    ASSERT_EQ(888, version.patchVersion());
    ASSERT_EQ(0, version.buildNumber());

    ASSERT_EQ(0, ZiosVersion::ziosVersionFromString(version, GOOD_VERSION1D));
    ASSERT_EQ(1, version.majorVersion());
    ASSERT_EQ(2, version.minorVersion());
    ASSERT_EQ(3, version.patchVersion());
    ASSERT_EQ(0, version.buildNumber());

    ASSERT_EQ(0, ZiosVersion::ziosVersionFromString(version, GOOD_VERSION1G));
    ASSERT_EQ(1, version.majorVersion());
    ASSERT_EQ(2, version.minorVersion());
    ASSERT_EQ(3, version.patchVersion());
    ASSERT_EQ(0, version.buildNumber());

    ASSERT_EQ(0, ZiosVersion::ziosVersionFromString(version, GOOD_VERSION1E));
    ASSERT_EQ(1, version.majorVersion());
    ASSERT_EQ(2, version.minorVersion());
    ASSERT_EQ(3, version.patchVersion());
    ASSERT_EQ(897, version.buildNumber());

    ASSERT_EQ(0, ZiosVersion::ziosVersionFromString(version, GOOD_VERSION1F));
    ASSERT_EQ(1, version.majorVersion());
    ASSERT_EQ(2, version.minorVersion());
    ASSERT_EQ(3, version.patchVersion());
    ASSERT_EQ(897, version.buildNumber());
}

TEST_F(WhenZiosVersionIsAskedToParseVersionString, shouldParseStringAndReturnErrorIfVersionIsInvalid) {
    ZiosVersion version;
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION1A));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION1B));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION1C));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION2A));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION2B));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION2C));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION3A));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION3B));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION3C));
    ASSERT_EQ(-1, ZiosVersion::ziosVersionFromString(version, BAD_VERSION3D));
}

class WhenZiosVersionIsAskedToCompareVersions : public testing::Test {
public:
    ~WhenZiosVersionIsAskedToCompareVersions() {}
};

TEST_F(WhenZiosVersionIsAskedToCompareVersions, shouldReturnGreaterThanWithNoBuildNumber) {
    ASSERT_TRUE(ZiosVersion(1, 2, 4).isNewerThan(ZiosVersion(1, 2, 3)));
    ASSERT_FALSE(ZiosVersion(1, 2, 3).isNewerThan(ZiosVersion(1, 2, 3)));
}

TEST_F(WhenZiosVersionIsAskedToCompareVersions, shouldReturnGreaterThanWithBuildNumber1) {
    ASSERT_TRUE(ZiosVersion(1, 2, 4, 345).isNewerThan(ZiosVersion(1, 2, 4)));
    ASSERT_TRUE(ZiosVersion(1, 2, 4, 345).isNewerThan(ZiosVersion(1, 2, 3, 123)));

    ASSERT_FALSE(ZiosVersion(1, 2, 4).isNewerThan(ZiosVersion(1, 2, 4, 1234)));
    ASSERT_FALSE(ZiosVersion(1, 2, 3, 123).isNewerThan(ZiosVersion(1, 2, 3, 123)));
}

TEST_F(WhenZiosVersionIsAskedToCompareVersions, shouldReturnGreaterThanWithBuildNumber2) {
    ASSERT_TRUE(ZiosVersion(1, 2, 4, 345).isNewerThan(ZiosVersion(1, 2, 4, 123)));
    ASSERT_FALSE(ZiosVersion(1, 2, 4, 123).isNewerThan(ZiosVersion(1, 2, 4, 123)));
}

TEST_F(WhenZiosVersionIsAskedToCompareVersions, shouldReturnLessThanWithNoBuildNumber) {
    ASSERT_TRUE(ZiosVersion(1, 2, 3).isOlderThan(ZiosVersion(1, 2, 4)));
    ASSERT_TRUE(ZiosVersion(1, 2, 3).isOlderThan(ZiosVersion(4, 2, 4)));

    ASSERT_FALSE(ZiosVersion(1, 2, 4).isOlderThan(ZiosVersion(1, 2, 3)));
    ASSERT_FALSE(ZiosVersion(1, 5, 4).isOlderThan(ZiosVersion(1, 2, 3)));
}

TEST_F(WhenZiosVersionIsAskedToCompareVersions, shouldReturnLessThanWithBuildNumber1) {
    ASSERT_TRUE(ZiosVersion(1, 2, 3, 234).isOlderThan(ZiosVersion(1, 2, 4, 456)));
    ASSERT_TRUE(ZiosVersion(1, 2, 3, 998).isOlderThan(ZiosVersion(1, 2, 4, 456)));
    ASSERT_TRUE(ZiosVersion(1, 2, 3, 234).isOlderThan(ZiosVersion(1, 2, 3, 235)));
    ASSERT_TRUE(ZiosVersion(1, 2, 3, 234).isOlderThan(ZiosVersion(4, 2, 4, 987)));

    ASSERT_FALSE(ZiosVersion(1, 2, 4, 345).isOlderThan(ZiosVersion(1, 2, 4, 345)));
    ASSERT_FALSE(ZiosVersion(1, 2, 4, 345).isOlderThan(ZiosVersion(1, 2, 3, 889)));
    ASSERT_FALSE(ZiosVersion(1, 5, 4, 678).isOlderThan(ZiosVersion(1, 2, 3, 99876)));
}

TEST_F(WhenZiosVersionIsAskedToCompareVersions, shouldReturnLessThanWithBuildNumber2) {
    ASSERT_TRUE(ZiosVersion(1, 2, 3, 123).isOlderThan(ZiosVersion(1, 2, 3)));
    ASSERT_TRUE(ZiosVersion(1, 2, 3).isOlderThan(ZiosVersion(1, 2, 4)));
    ASSERT_TRUE(ZiosVersion(1, 2, 3).isOlderThan(ZiosVersion(4, 2, 4)));

    ASSERT_FALSE(ZiosVersion(1, 2, 3).isOlderThan(ZiosVersion(1, 2, 3, 123)));
    ASSERT_FALSE(ZiosVersion(1, 2, 4).isOlderThan(ZiosVersion(1, 2, 3)));
    ASSERT_FALSE(ZiosVersion(1, 5, 4).isOlderThan(ZiosVersion(1, 2, 3)));
}

TEST_F(WhenZiosVersionIsAskedToCompareVersions, shouldReturnEqual) {
    ASSERT_TRUE(ZiosVersion(1, 2, 3).isTheSameAs(ZiosVersion(1, 2, 3)));
    ASSERT_FALSE(ZiosVersion(1, 2, 3).isTheSameAs(ZiosVersion(1, 2, 4)));
    ASSERT_FALSE(ZiosVersion(1, 2, 4).isTheSameAs(ZiosVersion(1, 2, 3)));
    ASSERT_TRUE(ZiosVersion(1, 2, 4).isTheSameAs(ZiosVersion(1, 2, 4)));
}

}  // namespace common_tests
}
