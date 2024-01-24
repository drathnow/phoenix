/** @file MockPasswordRule.h
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
#ifndef MOCKPASSWORDRULE_H_
#define MOCKPASSWORDRULE_H_
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <pwdrules.h>


class MockPasswordRule : public zios::common::PasswordRule {
public:
    ~MockPasswordRule() { destroyMockPasswordRule(); }

    MOCK_METHOD0(destroyMockPasswordRule, void());
    MOCK_CONST_METHOD1(isValidPassword, bool(const std::string& password));
    MOCK_CONST_METHOD0(description, const std::string&());

    void expectToBeDestroyed() {
        EXPECT_CALL(*this, destroyMockPasswordRule());
    }
};

#endif /* MOCKPASSWORDRULE_H_ */
