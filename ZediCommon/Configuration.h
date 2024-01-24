
/** @file Configuration.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 29, 2013 by daver:
 * IncrDev Jul 15, 2015 by eb: due to strangeness? in ARM GCC optimizer, changed const string to const char *
 * @endhistory
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <stdint.h>
#include <string>

#include "Utl.h"
#include "File.h"

struct cJSON;
namespace log4cplus { class Logger;}

#define CONNECTOR_MAX_PACKET_KEY                    "Connector:MaxPacketSize"
#define CONNECTOR_RETRY_STRATEGY_KEY                "Connector:RetryStrategy"
#define CONNECTOR_SERVER_ADDRESS_KEY                "Connector:ServerAddress"
#define CONNECTOR_SERVER_ADDRESS_LOCKED_KEY         "Connector:ServerAddress:Locked"
#define CONNECTOR_CONNECT_TIMEOUT                   "Connector:ConnectTimeoutSeconds"
#define CONNECTOR_REMOTE_PORT_KEY                   "Connector:RemotePort"
#define CONNECTOR_KEEPALIVE_ENABLED_KEY             "Connector:KeepAliveStrategy:Enable"
#define CONNECTOR_HEARTBEAT_TIME_SEC_KEY            "Connector:KeepAliveStrategy:HeartbeatSeconds"
#define CONNECTOR_HEARTBEAT_TIME_MIN_KEY            "Connector:KeepAliveStrategy:HeartbeatMinutes"
#define CONNECTOR_HEARTBEAT_INTERVAL_KEY            "Connector:KeepAliveStrategy:Interval"
#define CONNECTOR_KEEPALIVE_RESPONSE_TIMEOUT_KEY    "Connector:KeepAliveStrategy:ResponseTimeoutSeconds"
#define CONNECTOR_USERNAME_KEY                      "Connector:Authentication:Username"
#define CONNECTOR_HASH_KEY                          "Connector:Authentication:HashType"


#define RECYCLE_KEY                                 "Connector:RecycleTimer"
#define RECYCLE_ENABLED_KEY                         "Connector:RecycleTimer:Enable"
#define RECYCLE_INTERVAL_KEY                        "Connector:RecycleTimer:Interval"
#define RECYCLE_SHUTDOWN_INTERVAL_KEY               "Connector:RecycleTimer:ShutdownWaitInterval"
#define RECYCLE_TIME_MINUTES_KEY                    "Connector:RecycleTimer:Minutes"
#define RECYCLE_TIME_SECONDS_KEY                    "Connector:RecycleTimer:Seconds"

#define CONNECTOR_AUTHENTICAION_KEY                 "Connector:Authentication"
#define CONNECTOR_THREAD_POOL_SIZE_KEY              "Connector:ControlHandling:ThreadPoolSize"
#define CONNECTOR_RESPONSE_TIMEOUT_KEY              "Connector:ControlHandling:ResponseTimeoutSeconds"

#define TIMESYNC_KEY                                "TimeSync"
#define TIMESYNC_ENABLED_KEY                        "TimeSync:Enable"
#define TIMESYNC_ROUNTRIP_THRESHOLD_KEY             "TimeSync:RoundTripThreshold"
#define TIMESYNC_DEVELOPER_KEY                      "TimeSync:DeveloperMode"
#define TIMESYNC_HWCLOCK_THRESHOLD_KEY              "TimeSync:HWClockTimeSetThreshold"
#define TIMESYNC_INTERVAL_KEY                       "TimeSync:Interval"
#define TIMESYNC_ADJUST_THRESHOLD_KEY               "TimeSync:AdjustTimeThreshold"
#define TIMESYNC_MAXTIME_THRESHOLD_KEY              "TimeSync:MaxTimeThreshold"

#define OTAD_WATCHDOG_TIMEOUT_KEY                   "OTAD:WatchdogTimeout"
#define OTAD_RETRY_INTERVAL_KEY                     "OTAD:RetryInterval"
#define OTAD_RETRY_LIMIT_KEY                        "OTAD:RetryLimit"


namespace zios {
namespace common {

typedef std::string ConfigString;

class NotificationCenter;

class ConfigurationException : public Exception {
public:
    ConfigurationException(const char* funcName, int lineNumber, const std::string& message);
    ~ConfigurationException() throw();
};

class Configuration {
public:
    virtual ~Configuration();

    static const char* CONFIG_CHANGE_NOTIFICATION_NAME;

    Configuration& operator=(const Configuration& otherConfiguration);

    virtual bool valueAsBool(const char* itemKeyPath, bool defaultValue = false) const;
    virtual const char* valueAsString(const char* itemKeyPath, const char* defaultValue) const;
    virtual int32_t valueAsInt(const char* itemKeyPath, int32_t defaultValue) const;
    virtual double valueAsDouble(const char* itemKeyPath, double dfaultValue) const;
    virtual bool isKeyPresent(const char* keyPath) const;

    /** cjsonItemForKey
     *
     * Returns a pointer to a cJSON structure. The pointer is const for a reason!  Do not modify
     * anything in this structure or anything else.  Do not free this object.
     */
    virtual cJSON* cjsonItemForKey(const char* itemKeyPath) const;
    virtual bool setStringValueForKey(const char* value, const char* itemKeyPath);
    virtual bool setIntValueForKey(int value, const char* itemKeyPath);
    virtual bool setDoubleValueForKey(double value, const char* itemKeyPath);
    virtual std::string toString();

    virtual bool createStringKeyWithValue(const char* keyPath, const char* value);

    /**
     * Saves this configuration. Implemenations will do the actual saving of the configuration.
     *
     * @return true - configuration saved successfuly.  false if not. errno will contain the
     * reason.
     */
    virtual bool save() = 0;

    /**
     * Retrives a delta time string from a configuration key and converts it to the number
     * of seconds.
     *
     * @param config - Pointer to a Configuration object
     *
     * @param keyName - The name of the key containing the Delta Time string
     *
     * @param defaultValue - Default value to return if the key does not exist in the configuration
     *
     * @return Number of seconds represented by the value. -1 if the string is present but has invalid format
     */
    static int32_t deltaTimeValueFromConfig(Configuration* config, const char* keyName, int32_t defaultValue);

    /**
     * Retrieves an absolute time value from the configuration and converts it from a string
     * to a time_t.
     *
     * @param config - Pointer to a Configuration object
     *
     * @param keyName - The name of the key containing the Delta Time string
     *
     * @param defaultTime - Default value to return if the key does not exist in the configuration
     *
     * @return Absolute time (time_t). -1 if the string is present but has invalid format
     */
    static time_t absoluteTimeFromConfig(Configuration* config, const char* keyName, time_t defaultTime);

protected:
    Configuration();
    Configuration(const Configuration& otherConfiguration);

    cJSON* _root;
    bool _dirty;

    static const log4cplus::Logger _logger;
};

class StringConfiguration : public Configuration {
public:
    StringConfiguration(const ConfigString& jsonString);
    virtual ~StringConfiguration();

    virtual bool save();

};

class ConfigurationFileManager : Noncopyable {
public:
    ConfigurationFileManager(const File& configFile, uint32_t maxVersion = DEFAULT_MAX_VERSION);
    ConfigurationFileManager(const ConfigurationFileManager& otherManager);
    virtual ~ConfigurationFileManager();

    static const uint32_t DEFAULT_MAX_VERSION;

    ConfigurationFileManager& operator=(const ConfigurationFileManager& otherManager);

    virtual File& makeLatestConfigurationFile(File& newConfigFile);
    virtual File& latestConfigFile() { return _configFile; }

    bool operator==(const ConfigurationFileManager& otherManager) const {
        return this == &otherManager;
    }

protected:
    ConfigurationFileManager();

private:
    File _configFile;
    uint32_t _maxFileCount;

    void rename(File& f, uint32_t index);

    static const log4cplus::Logger _logger;
};

class FileConfiguration : Noncopyable, public Configuration {
public:
    FileConfiguration(NotificationCenter& notificationCenter, ConfigurationFileManager& fileManager);
    virtual ~FileConfiguration();

    virtual bool save();

private:
    NotificationCenter& _notificationCenter;
    ConfigurationFileManager& _fileManager;

    static const log4cplus::Logger _logger;
};


}
} /* namespace zios */
#endif /* CONFIGURATION_H_ */
