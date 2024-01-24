/** @file ZiosVersion.h
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
#ifndef ZIOSVERSION_H_
#define ZIOSVERSION_H_

#include <string>
#include <stdint.h>

namespace zios {
namespace common {

/**
 * ZoisVersion parser class separates the part of a version string and converts
 * them to integer values.  ZIOS version strings are specified as
 *
 *      <major>.<minor>.<patch>
 *
 */
class ZiosVersion {
public:
    ZiosVersion();
    ZiosVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t buildNumber = 0);
    ZiosVersion(const ZiosVersion& otherVersion);
    ~ZiosVersion();

    uint32_t majorVersion() const;
    uint32_t minorVersion() const;
    uint32_t patchVersion() const;
    uint32_t buildNumber() const;

    ZiosVersion& operator=(const ZiosVersion& otherVersion);

    std::string toString();

    bool isNewerThan(const ZiosVersion& otherVersion);
    bool isOlderThan(const ZiosVersion& otherVersion);
    bool isTheSameAs(const ZiosVersion& otherVersion);

    /**
     * Equality operator.  This operator is different than th isTheSameAs() method.  This
     * equality operator only checks if passed object is the same instance as this one.  It
     * does not compare any of the internal fields.
     *
     * @param otherVersion - The other version object
     *
     * @return true if the otherVersion instance is the same instance as this one.
     */
    bool operator==(const ZiosVersion& otherVersion);

    /**
     * Parses a version string of the form "<major>.<minor>.<patch>"
     *
     * @param returnVersion - The ZiosVersion object that will receive the parsed versions
     *
     * @param versionString - A version string of the form noted above
     *
     * @return 0 if parsing was successful. -1 if the version string has incorrect format, negative
     * numbers, or non-numeric values.
     */
    static int ziosVersionFromString(ZiosVersion& returnVersion, const std::string& versionString);

private:
    uint32_t _major;
    uint32_t _minor;
    uint32_t _patch;
    uint32_t _build;

    //
    //  These oeprators will not be overloaded
    //
    ZiosVersion& operator>(const ZiosVersion& otherVersion);
    ZiosVersion& operator<(const ZiosVersion& otherVersion);
    ZiosVersion& operator>=(const ZiosVersion& otherVersion);
    ZiosVersion& operator<=(const ZiosVersion& otherVersion);
};

} /* namespace common */
} /* namespace zios */

#endif /* ZIOSVERSION_H_ */
