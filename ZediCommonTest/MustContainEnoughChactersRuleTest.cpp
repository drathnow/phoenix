/** @file MustContainEnoughChactersRuleTest.cpp
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

#include <pwdrules.h>

namespace zios {
namespace common_tests {

using namespace zios::common;


class MustContainEnoughChactersRuleTest : public testing::Test {
public:
    virtual ~MustContainEnoughChactersRuleTest() {}
};

class WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithTooFewCharacters : public MustContainEnoughChactersRuleTest {
public:
    WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithTooFewCharacters() {}
    ~WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithTooFewCharacters() {}
};

TEST_F(WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithTooFewCharacters, shouldReturnFalse) {
    MustContainEnoughChactersRule rule(4);
    ASSERT_FALSE(rule.isValidPassword("123"));
}

class WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithEnoughCharacters : public MustContainEnoughChactersRuleTest {
public:
    WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithEnoughCharacters() {}
    ~WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithEnoughCharacters() {}
};

TEST_F(WhenMustContainEnoughChactersRuleIsAskedIfPasswordIsValidForStringWithTooFewCharacters, shouldReturnTrue) {
    MustContainEnoughChactersRule rule(4);
    ASSERT_TRUE(rule.isValidPassword("1234"));
}

}
}
