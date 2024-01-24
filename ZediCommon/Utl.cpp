/** @file Utl.cpp
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
 * IncrDev Aug 05, 2015 by eb: removed unneeded variable as per kw.
 * IncrDev Mar 18, 2015 by eb: add microsleep
 * @endhistory
 */

#include "Utl.h"

#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/statvfs.h>
#include <unistd.h>         // for sleep function
#include <cassert>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <cstdio>
#include <fcntl.h>

#include "Configuration.h"
#include "Exception.h"
#include "File.h"
#include "Hasher.h"
#include "StringUtl.h"

namespace zios {
namespace common {

using namespace std;

Noncopyable::Noncopyable()
{
}

Noncopyable::~Noncopyable()
{
}

Noncopyable::Noncopyable(const Noncopyable &otherCopyable)
{
}

Noncopyable& Noncopyable::operator=(const Noncopyable &otherCopyable)
{
    return *this;
}

PthreadWrapper::PthreadWrapper()
{
}

PthreadWrapper::~PthreadWrapper()
{
}

int PthreadWrapper::pthread_condattr_init(pthread_condattr_t *attr)
{
    return ::pthread_condattr_init(attr);
}
int PthreadWrapper::pthread_condattr_destroy(pthread_condattr_t *attr)
{
    return ::pthread_condattr_destroy(attr);
}

int PthreadWrapper::pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id)
{
    return ::pthread_condattr_setclock(attr, clock_id);
}

int PthreadWrapper::pthread_cond_timedwait(pthread_cond_t *conditionVariable, pthread_mutex_t *mutex, struct timespec *timeToWait)
{
    return ::pthread_cond_timedwait(conditionVariable, mutex, timeToWait);
}

int PthreadWrapper::pthread_cond_wait(pthread_cond_t *conditionVariable, pthread_mutex_t *mutex)
{
    return ::pthread_cond_wait(conditionVariable, mutex);
}

int PthreadWrapper::pthread_cond_init(pthread_cond_t *conditionVariable, pthread_condattr_t *condAttr)
{
    return ::pthread_cond_init(conditionVariable, condAttr);
}

int PthreadWrapper::pthread_cond_signal(pthread_cond_t *conditionVariable)
{
    return ::pthread_cond_signal(conditionVariable);
}

int PthreadWrapper::pthread_cond_broadcast(pthread_cond_t *conditionVariable)
{
    return ::pthread_cond_broadcast(conditionVariable);
}

int PthreadWrapper::pthread_cond_destroy(pthread_cond_t *conditionVariable)
{
    return ::pthread_cond_destroy(conditionVariable);
}

int PthreadWrapper::pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    return ::pthread_mutex_destroy(mutex);
}

uint8_t* Utl::newBufferWithValue(const uint8_t *value, uint32_t valueLength)
{
    assert(valueLength >= 0U);
    unsigned char *buffer = NULL;
    if (valueLength > 0)
    {
        buffer = new unsigned char[valueLength];
        ::memcpy((void*) buffer, value, valueLength);
    }
    return buffer;
}

void Utl::throwExceptionWithMessage(const std::string &message)
{
    throwExceptionWithMessage(errno, message);
}

void Utl::throwExceptionWithMessage(int errorNumber, const std::string &message)
{
    char messageBuffer[100];
    snprintf(messageBuffer, sizeof(messageBuffer), "%s. Error: %s(errno=%d)", message.c_str(), ::strerror(errorNumber),
            errorNumber);
    THROW_EXCEPTION(messageBuffer);
}

Hasher* Utl::hasherFromConfiguration(Configuration &configuration)
{
    const cJSON *hashConfig = configuration.cjsonItemForKey(CONNECTOR_HASH_KEY);
    if (hashConfig == NULL)
        THROW_NAMED_EXCEPTION(ConfigurationException, "Configuration is missing required key: " << CONNECTOR_HASH_KEY);
    return HashFactory::hasherForJsonConfig(hashConfig);
}

void Utl::createPidFile(std::string &pidFilePath)
{
    ofstream pidfileStream(pidFilePath.c_str(), ofstream::out | ofstream::trunc);
    pidfileStream << ::getpid() << endl;
    pidfileStream.close();
    File pidFile(pidFilePath);
    pidFile.deleteOnExit();
}

int Utl::serializeMapToFile(const std::map<std::string, std::string> &properties, File &file)
{
    int rc = -1;
    ofstream ostream(file.absolutePath(), ofstream::out);
    if (ostream)
    {
        for (map<string, string>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter)
            ostream << iter->first << "=" << iter->second << endl;
        ostream.close();
        rc = 0;
    }
    return rc;
}

int Utl::deserializeMapFromFile(std::map<std::string, std::string> &returnProperties, File &fromFile)
{
    int rc = -1;
    if (fromFile.exists())
    {
        ifstream istream(fromFile.absolutePath(), ofstream::in);
        if (istream)
        {
            string buffer;
            while (getline(istream, buffer))
                StringUtl::stringToMap(buffer, returnProperties);
            int saveErrno = errno;
            rc = (false == istream.eof()) ? -1 : 0;
            istream.close();
            errno = saveErrno;
        }
    }
    else
        errno = ENOENT;
    return rc;
}

int Utl::freeSpaceForPath(const char *path, uint64_t &retFreeSpace)
{
    struct statvfs statf;
    if (::statvfs(path, &statf) != 0)
        return -1;
    retFreeSpace = (statf.f_bfree * statf.f_bsize);
    return 0;
}

bool Utl::contentsOfFileAsString(File &file, std::string &contents, bool maintainLineBreaks)
{
    bool result = false;
    contents.clear();

    if (file.exists())
    {
        fstream stream;
        string line;
        stream.open((char*) file.name(), ios::in);
        while (stream.good())
        {
            getline(stream, line);
            contents.append(line);
            if (maintainLineBreaks)
                contents.append("\n");
        }
        result = true;
        stream.close();
    }
    return result;
}

const std::string Utl::bytesAsHexString(const uint8_t *byteArray, uint32_t byteArrayLength)
{
    return bytesAsHexString(byteArray, byteArrayLength, true);
}

const std::string Utl::bytesAsHexString(const uint8_t *byteArray, uint32_t byteArrayLength, bool whitespaceSep)
{
    std::stringstream stream;
    for (uint32_t i = 0; i < byteArrayLength; ++i)
    {
        stream << std::hex << std::setfill('0') << std::setw(2) << (int) byteArray[i];
        if (whitespaceSep)
            stream << " ";
    }
    return stream.str();
}

int Utl::hexStringAsBytes(const std::string &hexString, std::string &returnString)
{
    unsigned int c;
    const char *ptr = hexString.c_str();

    if (hexString.size() % 2 != 0)
        return -1;

    for (unsigned i = 0; i < hexString.size() / 2; i++)
    {
        if (isxdigit(ptr[0]) && isxdigit(ptr[1]))
        {
            ::sscanf(ptr, "%2X", &c);
            returnString.push_back(c);
            ptr += 2;
        }
        else
            return -1;
    }
    return hexString.size() / 2;
}

const std::string Utl::timeTtoString(time_t time)
{
    char buff[20];
    struct tm *tm = ::localtime(&time);
    if (tm == NULL)
        return "<localtime err>";
    ::strftime(buff, 20, "%Y-%m-%d %H:%M:%S", tm);
    return std::string(buff);
}

void Utl::millisleep(int msec)
{
    struct timespec rem;
    struct timespec t;
    t.tv_sec = msec / 1000;
    t.tv_nsec = (msec % 1000) * 1000000;
    rem.tv_sec = 0;
    rem.tv_nsec = 0;

    ::nanosleep(&t, &rem);
}

void Utl::timespecAddTime(struct timespec *tp, long milliseconds)
{
    // cost of a compare is cheaper than mods and division
    if (milliseconds >= 1000)
    {
        tp->tv_sec += milliseconds / 1000;
        tp->tv_nsec += (milliseconds % 1000) * 1000000L;
    }
    else
    {
        tp->tv_nsec += (milliseconds * 1000000L);
    }

    // handle rollover to keep tv_nsec valid
    if (tp->tv_nsec >= 1000000000)
    {
        tp->tv_sec += 1;
        tp->tv_nsec -= 1000000000;
    }
}

static const uint32_t SECS_PER_MIN = 60;
static const uint32_t SECS_PER_HOUR = 60 * SECS_PER_MIN;
static const uint32_t SECS_PER_DAY = 24 * SECS_PER_HOUR;

static const char* formattedTime(const char *fmt, time_t aTime, char *buf, size_t bufSize)
{
    struct tm *localTm = ::localtime(&aTime);
    if (localTm != NULL)
        return ::strftime(buf, bufSize, fmt, localTm) > 0 ? buf : NULL;
    return NULL;
}

DeltaTime::~DeltaTime()
{
}

int32_t DeltaTime::secondsForDeltaTimeString(const char *deltaTimeStr)
{
    int32_t seconds = -1;
    int days = 0;
    int hours = 0;
    int mins = 0;
    int secs = 0;
    if (::sscanf(deltaTimeStr, "%d %d:%d:%d", &days, &hours, &mins, &secs) < 4)
    {
        if (::sscanf(deltaTimeStr, "%d:%d:%d", &hours, &mins, &secs) == 3)
            seconds = hours * SECS_PER_HOUR + mins * SECS_PER_MIN + secs;
    }
    else
        seconds = days * SECS_PER_DAY + hours * SECS_PER_HOUR + mins * SECS_PER_MIN + secs;
    return seconds;
}

string DeltaTime::deltaTimeStringForSeconds(int seconds)
{
    int days = seconds / 86400;
    ostringstream oss;
    if (days > 0)
        oss << days << " ";
    oss << std::setfill('0') << std::setw(2) << (seconds % 86400) / 3600 << ":" << std::setfill('0') << std::setw(2)
            << (seconds % 3600) / 60 << ":" << std::setfill('0') << std::setw(2) << seconds % 60;
    return oss.str();
}

AbsoluteTime::~AbsoluteTime()
{
}

const char* AbsoluteTime::formattedISOTimeForTime(time_t aTime, char *buf, size_t bufSize)
{
    return formattedTime("%FT%XZ", aTime, buf, bufSize);
}

const char* AbsoluteTime::formattedTimeForTime(time_t aTime, char *buf, size_t bufSize)
{
    return formattedTime("%F %X", aTime, buf, bufSize);
}

time_t AbsoluteTime::absoluteTimeForString(const char *timeStr)
{
    struct tm tm;
    time_t result = -1;
    time_t now = ::time(NULL);
    struct tm *localTm = ::localtime(&now);
    if (localTm != NULL)
    {
        tm = *localTm;
        if (::strptime(timeStr, "%FT%X", &tm) == NULL)
        {
            if (::strptime(timeStr, "%X", &tm) == NULL)
            {
                if ((::strptime(timeStr, "%F", &tm)) != NULL)
                {
                    tm.tm_hour = 0;
                    tm.tm_min = 0;
                    tm.tm_sec = 0;
                    tm.tm_isdst = -1;
                    result = ::mktime(&tm);
                }
            }
            else
            {
                tm.tm_year = 70;
                tm.tm_mon = 0;
                tm.tm_yday = 0;
                tm.tm_wday = 0;
                tm.tm_mday = 1;
                tm.tm_isdst = -1;
                result = ::mktime(&tm);
            }
        }
        else
        {
            tm.tm_isdst = -1;
            result = ::mktime(&tm);
        }
    }
    return result;
}

const string& Utl::localEthernetAdapterName(std::string &retAddress)
{
    struct ifaddrs *addrs, *tmp;

    retAddress.clear();
    ::getifaddrs(&addrs);
    tmp = addrs;

    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
        {
            if (::strstr(tmp->ifa_name, "eth") != NULL)
            {
                retAddress = tmp->ifa_name;
                tmp = NULL;
                continue;
            }
        }
        tmp = tmp->ifa_next;
    }

    ::freeifaddrs(addrs);
    return retAddress;
}

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

void Utl::base64_encode(const uint8_t *buf, unsigned int bufLen, std::string &ret)
{
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (bufLen--)
    {
        char_array_3[i++] = *(buf++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }
}

vector<uint8_t> Utl::base64_decode(const string &encodedString)
{
    string decodedString;
    std::vector<uint8_t> ret;
    base64_decode(encodedString, decodedString);
    for (unsigned i = 0; i < decodedString.size(); i++)
        ret.push_back(decodedString[i]);
    return ret;
}

void Utl::base64_decode(const std::string &encodedString, std::string &returnBuffer)
{
    int in_len = encodedString.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];

    while (in_len-- && (encodedString[in_] != '=') && is_base64(encodedString[in_]))
    {
        char_array_4[i++] = encodedString[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                returnBuffer.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
        {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++)
        {
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
        {
            returnBuffer.push_back(char_array_3[j]);
        }
    } // if
}

unsigned int Utl::bitReverseInt(unsigned int input)
{
    unsigned int output = 0;
    int cnt = sizeof(int) * 8;
    for (int i = 0; i < cnt; i++)
    {
        if (((1 << i) & input) != 0)
            output = (output << 1) | 0x1;
        else
            output = output << 1;
    }
    return output;
}

unsigned int Utl::diffTimes(struct timespec *time1, struct timespec *time2)
{
    unsigned int result;
    result = (time1->tv_sec - time2->tv_sec) * 1000;            // sec to msec
    result += (time1->tv_nsec - time2->tv_nsec) / 1000000;      // nsec to msec
    return result;
}

int Utl::localIPv4AddressForDeviceName(std::string &retLocalAddress, const std::string &interfaceName)
{
    struct ifreq ifr;

    // Restrict type of address to IPv4
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ);

    // need any kind of interface socket handle for the ioctl
    int ifdev = ::socket(AF_INET, SOCK_DGRAM, 0);
    int ioctlresult = ::ioctl(ifdev, SIOCGIFADDR, &ifr);
    ::close(ifdev);

    if (ioctlresult < 0)
        return -1;

    // type casting is required as ifr.ifr_addr is a "generic" structure valid for multiple
    // types of network addresses.  First convert this to an internet address structure then
    // it can be decoded.  This is safe because earlier the "sa_family" was set to AF_INET
    retLocalAddress = ::inet_ntoa(((struct sockaddr_in*) &ifr.ifr_addr)->sin_addr);
    return 0;
}

#define HWADDR_len 6
const std::string& Utl::macAddressForInterface(std::string &retAddress, const char *interfaceName)
{
    struct ifreq ifr;

    ::memset(&ifr, 0, sizeof(struct ifreq));
    int s = ::socket(AF_INET, SOCK_DGRAM, 0);
    ::strcpy(ifr.ifr_name, interfaceName);
    ::ioctl(s, SIOCGIFHWADDR, &ifr);

    std::stringstream stream;
    for (uint32_t i = 0; i < HWADDR_len; ++i)
        stream << std::hex << std::setfill('0') << std::setw(2) << (int) ((unsigned char*) ifr.ifr_hwaddr.sa_data)[i] << ":";
    retAddress = stream.str();
    if (retAddress.length() > 1)
        retAddress.resize(retAddress.length() - 1);
    return retAddress;
}

} // namespace common
} /* namespace zios */

