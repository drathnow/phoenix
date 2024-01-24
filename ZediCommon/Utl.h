/** @file Utl.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 12, 2013 by daver:
 * IncrDev Apr 01, 2015 by eb: added base 64 static function
 * IncrDev Apr 01, 2015 by eb: added bitReverseInt static function
 * IncrDev Mar 18, 2015 by eb: add microsleep
 * @endhistory
 */

#ifndef UTL_H_
#define UTL_H_

#include <sys/time.h>
#include <stdint.h>
#include <cstddef>
#include <ctime>
#include <string>
#include <vector>
#include <pthread.h>
#include <cerrno>
#include <cstdio>
#include <map>

#define THROW_EXCEPTION_WITH_MSG(errNum, msg) \
    { \
        char messageBuffer[100]; \
        snprintf(messageBuffer, "%s. Error: %s(errno=%d)", message.c_str(), ::strerror(errorNumber), errorNumber); \
        throw Exception(messageBuffer); \
    }

namespace zios {
namespace common {

class File;
class Hasher;
class Configuration;

class Noncopyable {
public:
    Noncopyable();
    ~Noncopyable();

private:
    Noncopyable(const Noncopyable& otherCopyable);
    Noncopyable& operator=(const Noncopyable& otherCopyable);
};

class PthreadWrapper {
public:
    PthreadWrapper();
    virtual ~PthreadWrapper();

    virtual int pthread_condattr_init(pthread_condattr_t *attr);
    virtual int pthread_condattr_destroy(pthread_condattr_t *attr);
    virtual int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id);
    virtual int pthread_cond_timedwait(pthread_cond_t* conditionVariable, pthread_mutex_t* mutex, struct timespec *timeToWait);
    virtual int pthread_cond_wait(pthread_cond_t* conditionVariable, pthread_mutex_t* mutex);
    virtual int pthread_cond_init(pthread_cond_t* conditionVariable, pthread_condattr_t *cond_attr = NULL);
    virtual int pthread_cond_signal(pthread_cond_t* conditionVariable);
    virtual int pthread_cond_broadcast(pthread_cond_t* conditionVariable);
    virtual int pthread_cond_destroy(pthread_cond_t* conditionVariable);
    virtual int pthread_mutex_destroy(pthread_mutex_t* mutex);
};

class Utl {
public:
    static uint8_t* newBufferWithValue(const uint8_t* value, uint32_t valueLength);
    static void throwExceptionWithMessage(const std::string& message);
    static void throwExceptionWithMessage(int errorNumber, const std::string& message);

    static int hexStringAsBytes(const std::string& hexString, std::string& returnString);
    static const std::string timeTtoString(time_t time);

    static const std::string bytesAsHexString(const uint8_t* byteArray, uint32_t byteArrayLength);

    /**
     * Formats an array of bytes as a hex string.
     *
     * @param byteArray - Array of bytes.
     *
     * @param byteArrayLength - Length of the array
     *
     * @param whitespaceSep - true is whitepace separaters should be placed between each hex byte. false if
     * no spaces requested.
     *
     * @return string containing hex string.
     */
    static const std::string bytesAsHexString(const uint8_t* byteArray, uint32_t byteArrayLength, bool whitespaceSep);

    /**
     * Returns the contents of the specified file as a string. It's assumed that the specified file
     * is a text file and not binary.  If the file is binary, the returned contents might not
     * be what you expect.
     *
     * @return bool - true if the contents were read successfully, false if not.  The most
     * likely reason for a false return would be that the file does not exist.
     */
    static bool contentsOfFileAsString(File& file, std::string& contents, bool maintainLineBreaks = false);

    /**
     * Suspend processing for up to msec or if something signals the current thread
     * @param[in] msec Number of msec to sleep
     */
    static void millisleep(int msec);

    /**
     * Adds the specified milliseconds to a struct timespec.  Handles overflow cases to ensure the
     * timespec members remain valid.
     * @param[in,out] tp - pointer to a timespec
     * @param[in] milliseconds - number of milliseconds to add
     */
    static void timespecAddTime(struct timespec *tp, long milliseconds);

    /**
     * Encodes a byte array as a Base64 string.  Code was taken from StackOverflow, which
     * was a modification originally written by René Nyffenegger.
     *
     * @param buffer - Pointer to byte array to be encoded
     * @param bufferLength - Length of the byte array
     * @param returnBuffer - Return buffer for the encoded string. This buffer is cleared at
     * the start of this method
     */
    static void base64_encode(const uint8_t* buffer, unsigned int bufferLength, std::string& returnBuffer);

    /**
     * Decodes a base 64 encrypted string to a vector of bytes.  Code was taken from StackOverflow, which
     * was a modification originally written by René Nyffenegger.
     *
     * @param encodedString - A Base64 encoded string
     * @return vector
     */
    static std::vector<uint8_t> base64_decode(const std::string& encodedString);

    /**
     * Decodes a base 64 encrypted string to a vector of bytes.  Code was taken from StackOverflow, which
     * was a modification originally written by René Nyffenegger.
     *
     * @param encoded_string base 64 string
     * @param returnBuffer - String buffer that will receive the decoded bytes.  This buffer will be cleared
     * at the start of this method
     */
    static void base64_decode(const std::string& encodedString, std::string& returnBuffer);

    /**
     * Computes the difference, in milliseconds, between to times given as struct timespec object,
     * assuming time1 is bigger (later) than time2.
     *
     * @param time1 - Pointer to struct timespec object
     * @param time2 - Pointer to struct timespec object
     *
     * @return time difference in msec
     */
    static unsigned int diffTimes(struct timespec *time1, struct timespec *time2);

    /**
     * Returns the local address for the given device name
     *
     * @param retLocalAddress - Place to return the address
     * @param interfaceName - The device name.  If not specified,
     * @return 0 - Everything was happy. -1
     */
    static int localIPv4AddressForDeviceName(std::string& retLocalAddress, const std::string& interfaceName = "eth0");

    static const std::string& macAddressForInterface(std::string& retAddress, const char* interfaceName);

    /**
     * reverse an integer eg: 52 will turn into 11.... (think binary)
     * @param[in] input   integer to reverse
     * @return integer reversed.
     */
    static unsigned int bitReverseInt(unsigned int input);

    static Hasher* hasherFromConfiguration(Configuration& configuration);

    static void createPidFile(std::string& pidFilePath);

    static int serializeMapToFile(const std::map<std::string, std::string>& properties, File& toFile);

    static int deserializeMapFromFile(std::map<std::string, std::string>& returnProperties, File& fromFile);


    /**
     * Returns the free space, in bytes, on a partition containing a path.  If the path specifies either
     * a file or a directory, then it must exists otherwise the method will fail.
     *
     * @param path - The path for the partition.
     * @param retFreeSpace - Reference to variable that will receive the free space value
     * @return 0 if successful. -1 if not. errno contains reason
     */
    static int freeSpaceForPath(const char* path, uint64_t& retFreeSpace);

    /**
     * Returns the name of the local Ethernet (eth) device. If there are more than one device on the system,
     * this method will return the first one it finds.
     *
     * @param retBuf - Buffer to receive the name
     * @param bufSize
     * @return The length of the string returned in retAddress.  0 if no ethernet device could be found
     */
    static const std::string& localEthernetAdapterName(std::string& retAddress);

private:
    Utl();
    ~Utl();
};


class DeltaTime {
public:
    ~DeltaTime();

    /**
     * Parses a delta time specification.  The time can be specified as follows:
     *
     *    [days] HH:MM:SS
     *
     *    where
     *        days - Is optional. It is a number of days
     *        HH   - Number of hours
     *        DD   - Number of days
     *        SS   - Number of seconds
     *
     * With the exceptoin of days, all values must be specified. There is no restriction
     * on the values specified for hours, minutes, and seconds other than they must be valid
     * signed numbers.  So the following are valid:
     *
     *    00:00:60  - 60 seconds
     *    100:00:15 - 100 hours, 0 Minitues, 15 seconds = 360015 seconds.
     *
     * @return -1 if the format string was invalid or could not be parsed or
     * number of seconds;
     */
    static int32_t secondsForDeltaTimeString(const char* deltaTimeStr);

    /**
     * Converts number of seconds, as an int, into a delta time string
     *
     * @param[in] seconds
     *
     * @return A delta time string
     */
    static std::string deltaTimeStringForSeconds(int seconds);

private:
    DeltaTime();
};

class AbsoluteTime {
public:
    ~AbsoluteTime();

    /**
     * Parses an absolute time value expressed as an ISO date.  That time
     * can be either a full date specification, i.e.:
     *
     *      YYYY-MM-DDTHH:MM:SS e.g. 2015-03-23T10:23:45
     *
     *  or just a time spec:
     *
     *      HH:MM:SS e.g. 10:23:45
     *
     *  If only a time is specified, then the start date is assumed to be January 1, 1970.
     *
     * @param[in] timeStr - A time expressed as ISO time.
     *
     * @return time_t - The time_t representation of the time string. -1 if the
     * timeStr argument is invalid
     */
    static time_t absoluteTimeForString(const char* timeStr);

    /**
     * Formats a given time as a IOS standard time. (i.e. YYYY-MM-DDTHH:mm:ssZ)
     *
     * @param[in] aTime the time to format
     * @param[out] buf the buffer to return the formatted time
     * @param[in] bufSize the size of the buf argument
     * @return pointer to a formatted time string (buf).  NULL if the buffer is too small
     */
    static const char* formattedISOTimeForTime(time_t aTime, char* buf, size_t bufSize);

    /**
     * Formats a given time as a non-IOS standard time. (i.e. YYYY-MM-DD HH:mm:ss)
     *
     * @param[in] aTime the time to format
     * @param[out] buf the buffer to return the formatted time
     * @param[in] bufSize the size of the buf argument
     * @return pointer to a formatted time string (buf).  NULL if the buffer is too small
     */
    static const char* formattedTimeForTime(time_t aTime, char* buf, size_t bufSize);

private:
    AbsoluteTime();
};


} // namespace common
} // namespace zios
#endif /* UTL_H_ */
