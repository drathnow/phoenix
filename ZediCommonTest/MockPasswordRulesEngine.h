/** @file MockPasswordRulesEngine.h
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
#ifndef MOCKPASSWORDRULESENGINE_H_
#define MOCKPASSWORDRULESENGINE_H_
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pwdrules.h>

namespace zios {
namespace common {

class MockPasswordRulesEngine : public zios::common::PasswordRulesEngine {
public:
    MockPasswordRulesEngine() {}
    ~MockPasswordRulesEngine() { destroyMockPasswordRulesEngine(); }

    MOCK_METHOD0(destroyMockPasswordRulesEngine, void());
    MOCK_CONST_METHOD1(isValidPassword, bool(const std::string& password));
    MOCK_CONST_METHOD1(descriptions, int(std::vector<std::string>& returnedDescriptions));

    void expectToBeDestroyed() {
        EXPECT_CALL(*this, destroyMockPasswordRulesEngine());
    }
};

}
}

#endif /* MOCKPASSWORDRULESENGINE_H_ */
