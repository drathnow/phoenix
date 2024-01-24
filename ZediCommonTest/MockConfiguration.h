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
 * Created Mar 7, 2016 by daver:
 * @endhistory
 */
#ifndef MOCKCONFIGURATION_H_
#define MOCKCONFIGURATION_H_
#include <gmock/gmock.h>
#include <Configuration.h>

namespace zios {
namespace common {

class MockConfiguration : public Configuration {
public:
    MockConfiguration() {}
    ~MockConfiguration() {}

    MOCK_CONST_METHOD1(isKeyPresent, bool(const char* keyPath));
    MOCK_CONST_METHOD2(valueAsString, const char*(const char* itemKeyPath, const char* defaultValue));
    MOCK_METHOD2(setStringValueForKey, bool(const char* value, const char* itemKeyPath));
    MOCK_METHOD2(createStringKeyWithValue, bool(const char* itemKeyPath, const char* value));
    MOCK_METHOD0(save, bool());
};

}
}


#endif /* CONFIGURATION_H_ */
