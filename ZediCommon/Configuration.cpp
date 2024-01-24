/** @file Configuration.cpp
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
 * BugFix_ Jan 08, 2015 by eb:  change delete _root to cJSON_Delete(_root) in Configuration test
 * IncrDev Jul 15, 2015 by eb: due to strangeness? in ARM GCC optimizer, changed const string to const char *
 * @endhistory
 */

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/streams.h>
#include <memory>
#include <map>
#include <fstream>
#include <string>
#include <cstring>
#include <utility>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "NotificationCenter.h"
#include <cjson/cJSON.h>
#include "Configuration.h"

namespace zios {
namespace common {

using namespace std;
using namespace log4cplus;

static const string KEY_DELIMITER(":");
const Logger Configuration::_logger = Logger::getInstance("zios.common.Configuration");
const Logger FileConfiguration::_logger = Logger::getInstance("zios.common.FileConfiguration");
const Logger ConfigurationFileManager::_logger = Logger::getInstance("zios.common.ConfigurationFileManager");

const char* Configuration::CONFIG_CHANGE_NOTIFICATION_NAME("ConfigurationChanged");
const uint32_t ConfigurationFileManager::DEFAULT_MAX_VERSION = 3;


static cJSON* createKeyPath(const string& itemKeyPath, cJSON* root) {
    cJSON* nextRoot = NULL;
    string::size_type next = itemKeyPath.find_first_of(KEY_DELIMITER, 0);
    if (next == string::npos) {
        return root;
    } else {
        string::size_type whatsLeft = itemKeyPath.size() - (next+1);
        nextRoot = cJSON_GetObjectItem(root, itemKeyPath.substr(0, next).c_str());
        if (nextRoot == NULL)
            nextRoot = cJSON_AddObjectToObject(root, itemKeyPath.substr(0, next).c_str());
        string nextKeyPath = itemKeyPath.substr(next+1, whatsLeft);
        return createKeyPath(nextKeyPath.c_str(), nextRoot);
    }
}

static cJSON* getItemFromRoot(const string& itemKeyPath, cJSON* root) {
	string::size_type next = itemKeyPath.find_first_of(KEY_DELIMITER, 0);
	if (next == string::npos)
		return cJSON_GetObjectItem(root, itemKeyPath.c_str());
	else {
	    string::size_type whatsLeft = itemKeyPath.size() - (next+1);
		cJSON* nextRoot = cJSON_GetObjectItem(root, itemKeyPath.substr(0, next).c_str());
		if (nextRoot == NULL)
		    return NULL;
		string nextKeyPath = itemKeyPath.substr(next+1, whatsLeft);
		return getItemFromRoot(nextKeyPath.c_str(), nextRoot);
	}
}


ConfigurationException::ConfigurationException(const char* funcName, int lineNumber, const std::string& message) : Exception(funcName, lineNumber, message) {
}

ConfigurationException::~ConfigurationException() throw() {
}

Configuration::Configuration() : _root(NULL), _dirty(false) {
}

Configuration::~Configuration() {
    if (NULL != _root)
        cJSON_Delete(_root);
}

Configuration::Configuration(const Configuration& otherConfiguration) : _root(otherConfiguration._root), _dirty(false) {
}

Configuration& Configuration::operator=(const Configuration& otherConfiguration) {
    _root = otherConfiguration._root;
    return *this;
}

bool Configuration::valueAsBool(const char* itemKeyPath, bool defaultValue) const {
    string skey(itemKeyPath);
    cJSON* item = getItemFromRoot(skey, _root);
    return item == NULL ? defaultValue : ((::strcasecmp("true", item->valuestring) == 0 || ::strcasecmp("yes", item->valuestring) == 0));
}

const char* Configuration::valueAsString(const char* itemKeyPath, const char* defaultValue) const {
    string skey(itemKeyPath);
    cJSON* item = getItemFromRoot(skey, _root);
    return item == NULL ? defaultValue : item->valuestring;
}

int32_t Configuration::valueAsInt(const char* itemKeyPath, int32_t defaultValue) const {
    string skey(itemKeyPath);
    cJSON* item = getItemFromRoot(skey, _root);
    return item == NULL ? defaultValue : item->valueint;
}

double Configuration::valueAsDouble(const char* itemKeyPath, double defaultValue) const {
    string skey(itemKeyPath);
    cJSON* item = getItemFromRoot(skey, _root);
    return item == NULL ? defaultValue : item->valuedouble;
}

bool Configuration::isKeyPresent(const char* keyPath) const {
    return cjsonItemForKey(keyPath) != NULL;
}

cJSON* Configuration::cjsonItemForKey(const char* itemKeyPath) const {
    string skey((const char*)itemKeyPath);
    return getItemFromRoot(skey, _root);
}

std::string Configuration::toString() {
    if (_root == NULL)
        return "<null>";
    unique_ptr<char> result(cJSON_Print(_root));
    result.get();
    return string(result.get() == NULL ? "<null>" : result.get());
}

bool Configuration::createStringKeyWithValue(const char* keyPath, const char* value) {
    string itemPath(keyPath);
    cJSON* json = createKeyPath(keyPath, _root);
    string::size_type pos = itemPath.find_last_of(KEY_DELIMITER);
    if (pos > 0)
        pos += 1;
    if (cJSON_AddStringToObject(json, itemPath.substr(pos).c_str(), value) != NULL) {
        _dirty = true;
        return true;
    }
    return false;
}

bool Configuration::setStringValueForKey(const char* value, const char* itemKeyPath) {
    string skey((char*)itemKeyPath);
    string::size_type pos = skey.find_last_of(KEY_DELIMITER);
    if (pos == string::npos) {
        cJSON* oldItem = cJSON_GetObjectItem(_root, skey.c_str());
        if (oldItem != NULL && oldItem->type == cJSON_String) {
            cJSON* newItem = cJSON_CreateString((const char*)value);
            cJSON_ReplaceItemInObject(_root, skey.c_str(), newItem);
            _dirty = true;
            return true;
        }
    } else {
        string name(skey.substr(pos+1));
        cJSON* item = getItemFromRoot(skey.substr(0, pos), _root);
        if (item != NULL) {
            cJSON* oldItem = cJSON_GetObjectItem(item, name.c_str());
            if (oldItem != NULL && oldItem->type == cJSON_String) {
                cJSON* newItem = cJSON_CreateString((const char*)value);
                cJSON_ReplaceItemInObject(item, name.c_str(), newItem);
                _dirty = true;
                return true;
            }
        }
    }
    return false;

}

bool Configuration::setIntValueForKey(int value, const char* itemKeyPath) {
    string skey((char*)itemKeyPath);
    cJSON* item = getItemFromRoot(skey, _root);
    if (item != NULL && item->type == cJSON_Number) {
        item->valueint = value;
        item->valuedouble = value;
        _dirty = true;
        return true;
    }
    return false;
}

bool Configuration::setDoubleValueForKey(double value, const char* itemKeyPath) {
    string skey((char*)itemKeyPath);
    cJSON* item = getItemFromRoot(skey, _root);
    if (item != NULL && item->type == cJSON_Number) {
        item->valueint = value;
        item->valuedouble = value;
        _dirty = true;
        return true;
    }
    return false;
}


int32_t Configuration::deltaTimeValueFromConfig(Configuration* config, const char* keyName, int32_t defaultValue) {
    const char* deltaTimeStr = config->valueAsString(keyName, NULL);
    if (deltaTimeStr == NULL)
        return defaultValue;
    int32_t interval = DeltaTime::secondsForDeltaTimeString(deltaTimeStr);
    if (-1 == interval) {
        LOG4CPLUS_WARN(_logger, "Value for " << keyName << " has invalid delta time format.");
        return -1;
    }
    return interval;
}

time_t Configuration::absoluteTimeFromConfig(Configuration* config, const char* keyName, time_t defaultTime) {
    const char* absoluteTimeStr = config->valueAsString(keyName, NULL);
    if (absoluteTimeStr == NULL)
        return defaultTime;
    time_t absoluteTime = AbsoluteTime::absoluteTimeForString(absoluteTimeStr);
    if (-1 == absoluteTime) {
        LOG4CPLUS_WARN(_logger, "Value for " << keyName << " has invalid absolute time format.");
        return -1;
    }
    return absoluteTime;
}


StringConfiguration::StringConfiguration(const ConfigString& jsonString) {
    _root = cJSON_Parse(jsonString.c_str());
}

StringConfiguration::~StringConfiguration() {
}

bool StringConfiguration::save() {
    return true;
}

ConfigurationFileManager::ConfigurationFileManager() :
    _configFile(""),
    _maxFileCount(0) {
}

ConfigurationFileManager::ConfigurationFileManager(const File& configFile, uint32_t maxVersion) :
    _configFile(configFile),
    _maxFileCount(maxVersion) {
}

ConfigurationFileManager& ConfigurationFileManager::operator=(const ConfigurationFileManager& otherManager) {
    if (this != &otherManager) {
        _configFile = otherManager._configFile;
        _maxFileCount = otherManager._maxFileCount;
    }
    return *this;
}

ConfigurationFileManager::~ConfigurationFileManager() {
}

void ConfigurationFileManager::rename(File& existingFile, uint32_t index) {
    if (index > _maxFileCount) {
        existingFile.turf();
    } else {
        ostringstream oss;
        oss << _configFile.absolutePath() << "." << index;
        File file(oss.str().c_str());
        if (file.exists())
            rename(file, index+1);
        LOG4CPLUS_DEBUG(_logger, "Renaming " << existingFile.absolutePath() << " to " << file.absolutePath());
        existingFile.renameTo(file);
    }
}

File& ConfigurationFileManager::makeLatestConfigurationFile(File& newConfigFile) {
    ostringstream oss;
    rename(_configFile, 1);
    newConfigFile.renameTo(_configFile);
    return _configFile;
}

FileConfiguration::FileConfiguration(NotificationCenter& notificationCenter, ConfigurationFileManager& fileManager) :
        _notificationCenter(notificationCenter),
        _fileManager(fileManager) {
    string::size_type size = fileManager.latestConfigFile().size();
    char buffer[size];
    fstream fstream((char*)fileManager.latestConfigFile().absolutePath(), ios_base::in);
    fstream.read((char*)buffer, size);
    buffer[fstream.gcount()] = '\0';
    fstream.close();
    _root = cJSON_Parse((const char*)buffer);
    if (_root == NULL)
        THROW_NAMED_EXCEPTION(ConfigurationException, "JSON Configuration is not valid:" << endl << (const char*)buffer);
    LOG4CPLUS_INFO(_logger, "Configuration loaded from " << fileManager.latestConfigFile().absolutePath());
}

FileConfiguration::~FileConfiguration() {
}

bool FileConfiguration::save() {
    if (_dirty) {
        File parent = _fileManager.latestConfigFile().parentFile();
        File tmpFile = File::createTempFile("config", ".tmp", &parent);
        fstream fstream((char*)tmpFile.absolutePath(), ios_base::out);
        if (fstream.fail() == false) {
            char * config = cJSON_Print(_root);
            if (config != NULL) {
                fstream.write(config, ::strlen(config));
                ::free(config);
                if (fstream.fail() == false) {
                    fstream.close();
                    _fileManager.makeLatestConfigurationFile(tmpFile);
                    _dirty = false;
                    _notificationCenter.postNotification(CONFIG_CHANGE_NOTIFICATION_NAME);
                    LOG4CPLUS_INFO(_logger, "Configuration saved to " << _fileManager.latestConfigFile().absolutePath());
                    return true;
                } else {
                    LOG4CPLUS_ERROR(_logger, "Unable to format configuration as JSON string");
                    return true;
                }
            }
        }
    } else
        return true;
    return false;
}

}
} /* namespace zios */
