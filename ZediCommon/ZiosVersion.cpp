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
#include "ZiosVersion.h"
#include "StringUtl.h"

#include <sstream>
#include <string>
#include <istream>

namespace zios {
namespace common {

using namespace std;

static const char DOT_DELMITER = '.';
static const char DASH_DELMITER = '-';

ZiosVersion::ZiosVersion() : _major(0), _minor(0), _patch(0), _build(0) {
}

ZiosVersion::ZiosVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t buildNumber) : _major(major), _minor(minor), _patch(patch), _build(buildNumber) {
}

ZiosVersion::ZiosVersion(const ZiosVersion& otherVersion) :
        _major(otherVersion._major),
        _minor(otherVersion._minor),
        _patch(otherVersion._patch),
        _build(otherVersion._build) {
}

ZiosVersion::~ZiosVersion() {
}

ZiosVersion& ZiosVersion::operator=(const ZiosVersion& otherVersion) {
    _major = otherVersion._major;
    _minor = otherVersion._minor;
    _patch = otherVersion._patch;
    _build = otherVersion._build;
    return *this;
}

std::string ZiosVersion::toString() {
    stringstream sstream;
    sstream << _major << "."
            << _minor << "."
            << _patch;
    if (_build > 0)
        sstream << "." << _build;
    return sstream.str();
}

bool ZiosVersion::isNewerThan(const ZiosVersion& otherVersion) {
    if (_major > otherVersion._major)
        return true;
    if (_major == otherVersion._major) {
        if (_minor > otherVersion._minor)
            return true;
        if (_minor == otherVersion._minor) {
            if (_patch > otherVersion._patch)
                return true;
            if (_patch == otherVersion._patch) {
                if (_build != 0 && otherVersion._build == 0)
                    return true;
                if ((_build == 0 && otherVersion._build == 0) ||(_build == 0 && otherVersion._build > 0))
                    return false;
                return _build > otherVersion._build;
            }
        }
    }
    return false;
}

bool ZiosVersion::isOlderThan(const ZiosVersion& otherVersion) {
    if (_major < otherVersion._major)
        return true;
    if (_major == otherVersion._major) {
        if (_minor < otherVersion._minor)
            return true;
        if (_minor == otherVersion._minor) {
            if (_patch < otherVersion._patch)
                return true;
            if (_patch == otherVersion._patch) {
                if (_build > 0 && otherVersion._build == 0)
                    return true;
                if ((_build == 0 && otherVersion._build > 0) || (_build == 0 && otherVersion._build == 0))
                    return false;
                return _build < otherVersion._build;
            }
        }
    }
    return false;
}

bool ZiosVersion::isTheSameAs(const ZiosVersion& otherVersion) {
    return _major == otherVersion._major
           && _minor == otherVersion._minor
           && _patch == otherVersion._patch
           && _build == otherVersion._build;
}

bool ZiosVersion::operator==(const ZiosVersion& otherVersion) {
    return this == &otherVersion;
}

uint32_t ZiosVersion::majorVersion() const {
    return _major;
}

uint32_t ZiosVersion::minorVersion() const {
    return _minor;
}

uint32_t ZiosVersion::patchVersion() const {
    return _patch;
}

uint32_t ZiosVersion::buildNumber() const {
    return _build;
}

int ZiosVersion::ziosVersionFromString(ZiosVersion& version, const std::string& versionString) {
    int32_t tmp;
    string token;
    istringstream sourceStream(versionString);

    //
    // First pull the major version
    //
    if (!getline(sourceStream, token, DOT_DELMITER))
        return -1;
    StringUtl::trim(token);
    if (token.size() == 0)
        return -1;
    if (false == StringUtl::stringToInt(token, tmp) || tmp < 0)
        return -1;
    version._major = tmp;

    //
    // Then the minor version
    //
    if (!getline(sourceStream, token, DOT_DELMITER))
        return -1;
    StringUtl::trim(token);
    if (token.size() == 0)
        return -1;
    if (false == StringUtl::stringToInt(token, tmp) || tmp < 0)
        return -1;
    version._minor = tmp;

    //
    // Then the patch version.  The patch may have an extra qualifier after
    // it so we need to check for it before proceding
    //
    if (!getline(sourceStream, token, DOT_DELMITER))
        return -1;
    string::size_type pos = string::npos;
    if (string::npos != (pos = token.find(DASH_DELMITER)))
        token = token.substr(0, pos);
    StringUtl::trim(token);
    if (token.size() == 0)
        return -1;
    if (false == StringUtl::stringToInt(token, tmp) || tmp < 0)
        return -1;
    version._patch = tmp;

    //
    // Do we have a build number?
    //
    if (getline(sourceStream, token, DOT_DELMITER)) {
        string::size_type pos = string::npos;
        if (string::npos != (pos = token.find(DASH_DELMITER)))
            token = token.substr(0, pos);
        StringUtl::trim(token);
        if (token.size() == 0)
            return -1;
        if (false == StringUtl::stringToInt(token, tmp) || tmp < 0)
            return -1;
        version._build = tmp;
    }

    return 0;
}



} /* namespace common */
} /* namespace zios */
