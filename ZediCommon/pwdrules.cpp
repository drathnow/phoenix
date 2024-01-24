/** @file pwdrules.cpp
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
#include <sstream>
#include "pwdrules.h"

namespace zios {
namespace common {

using namespace std;

PasswordRule::PasswordRule() {
}

PasswordRule::~PasswordRule() {
}


MustContainEnoughChactersRule::MustContainEnoughChactersRule(int requiredCharCount) : _requiredCharCount(requiredCharCount) {
    stringstream stream;
    stream << "Password must contain at least " << requiredCharCount << " characters";
    _description = stream.str();
}

MustContainEnoughChactersRule::~MustContainEnoughChactersRule() {
}

bool MustContainEnoughChactersRule::isValidPassword(const std::string& password) const {
    return password.size() >= (unsigned)_requiredCharCount;
}

PasswordRulesEngine::PasswordRulesEngine() {
}

PasswordRulesEngine::PasswordRulesEngine(std::vector<PasswordRule*>& rules) {
    for (vector<PasswordRule*>::const_iterator iter = rules.begin(); iter != rules.end(); ++iter)
        _passwordRules.push_back((*iter));
    rules.clear();
}

PasswordRulesEngine::~PasswordRulesEngine() {
    while (_passwordRules.size() > 0) {
        PasswordRule* p = _passwordRules.back();
        _passwordRules.pop_back();
        delete p;
    }
}

bool PasswordRulesEngine::isValidPassword(const std::string& password) const {
    for (vector<PasswordRule*>::const_iterator iter = _passwordRules.begin(); iter != _passwordRules.end(); ++iter) {
        if ((*iter)->isValidPassword(password) == false)
            return false;
    }
    return true;
}


int PasswordRulesEngine::descriptions(std::vector<std::string>& returnedDescriptions) const {
    for (vector<PasswordRule*>::const_iterator iter = _passwordRules.begin(); iter != _passwordRules.end(); ++iter)
        returnedDescriptions.push_back((*iter)->description());
    return returnedDescriptions.size();
}


PasswordRulesEngine* PasswordRulesEngine::newPasswordRulesEngine() {
    vector<PasswordRule*> rules;
    rules.push_back(new MustContainEnoughChactersRule(6));
    return new PasswordRulesEngine(rules);
}


} /* namespace web */
} /* namespace zios */
