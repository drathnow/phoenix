/** @file PasswordRulesEngineTest.cpp
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
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>

#include "MockPasswordRule.h"

namespace zios {
namespace common_tests {

using testing::Mock;
using testing::Return;
using testing::ReturnRef;

using namespace std;
using namespace zios::common;

class PasswordRulesEngineTest : public testing::Test {
public:
    PasswordRulesEngineTest() {}
    ~PasswordRulesEngineTest() {}

    static const std::string PWD;
};

const std::string PasswordRulesEngineTest::PWD("FOO");

class WhenPasswordRulesEngineIsDestroyed : public PasswordRulesEngineTest {
public:
    ~WhenPasswordRulesEngineIsDestroyed() {}
};

TEST_F(WhenPasswordRulesEngineIsDestroyed, shouldDeleteAllPasswordRules) {
    MockPasswordRule* rule1 = new MockPasswordRule();
    MockPasswordRule* rule2 = new MockPasswordRule();

    EXPECT_CALL(*rule1, destroyMockPasswordRule());
    EXPECT_CALL(*rule2, destroyMockPasswordRule());

    std::vector<PasswordRule*> vec;
    vec.push_back(rule1);
    vec.push_back(rule2);
    {
        PasswordRulesEngine engine(vec);
        ASSERT_EQ(0, vec.size());
    }
    Mock::VerifyAndClearExpectations(rule1);
    Mock::VerifyAndClearExpectations(rule2);
}

class WhenPasswordRulesEngineIsAskedToValidateAPasswordThatIsValid : public PasswordRulesEngineTest {
public:
    ~WhenPasswordRulesEngineIsAskedToValidateAPasswordThatIsValid() {}
};

TEST_F(WhenPasswordRulesEngineIsAskedToValidateAPasswordThatIsValid, shouldRunThuAllRulesAndReturnTrue) {

    MockPasswordRule* rule1 = new MockPasswordRule();
    MockPasswordRule* rule2 = new MockPasswordRule();

    EXPECT_CALL(*rule1, destroyMockPasswordRule());
    EXPECT_CALL(*rule2, destroyMockPasswordRule());
    EXPECT_CALL(*rule1, isValidPassword(PWD)).WillOnce(Return(true));
    EXPECT_CALL(*rule2, isValidPassword(PWD)).WillOnce(Return(true));

    std::vector<PasswordRule*> vec;
    vec.push_back(rule1);
    vec.push_back(rule2);
    PasswordRulesEngine engine(vec);

    ASSERT_TRUE(engine.isValidPassword(PWD));
}

class WhenPasswordRulesEngineIsAskedToValidateAPasswordThatIsInValid : public PasswordRulesEngineTest {
public:
    ~WhenPasswordRulesEngineIsAskedToValidateAPasswordThatIsInValid() {}
};

TEST_F(WhenPasswordRulesEngineIsAskedToValidateAPasswordThatIsInValid, shouldFailOnFirstRuleFailure) {

    MockPasswordRule* rule1 = new MockPasswordRule();
    MockPasswordRule* rule2 = new MockPasswordRule();

    EXPECT_CALL(*rule1, destroyMockPasswordRule());
    EXPECT_CALL(*rule2, destroyMockPasswordRule());
    EXPECT_CALL(*rule1, isValidPassword(PWD)).WillOnce(Return(false));
    EXPECT_CALL(*rule2, isValidPassword(PWD)).Times(0);

    std::vector<PasswordRule*> vec;
    vec.push_back(rule1);
    vec.push_back(rule2);
    PasswordRulesEngine engine(vec);

    ASSERT_FALSE(engine.isValidPassword(PWD));
}

class WhenPasswordRulesEngineIsAskedToReturnListOfRulesDescriptions : public PasswordRulesEngineTest {
public:
    ~WhenPasswordRulesEngineIsAskedToReturnListOfRulesDescriptions() {}
};

TEST_F(WhenPasswordRulesEngineIsAskedToReturnListOfRulesDescriptions, shouldReturnDescriptionList) {

    const std::string desc1;
    const std::string desc2;

    MockPasswordRule* rule1 = new MockPasswordRule();
    MockPasswordRule* rule2 = new MockPasswordRule();

    EXPECT_CALL(*rule1, destroyMockPasswordRule());
    EXPECT_CALL(*rule2, destroyMockPasswordRule());
    EXPECT_CALL(*rule1, description()).WillOnce(ReturnRef(desc1));
    EXPECT_CALL(*rule2, description()).WillOnce(ReturnRef(desc2));

    std::vector<PasswordRule*> rules;
    rules.push_back(rule1);
    rules.push_back(rule2);
    PasswordRulesEngine engine(rules);

    std::vector<std::string> vec;
    ASSERT_EQ(2, engine.descriptions(vec));
}


}
}
