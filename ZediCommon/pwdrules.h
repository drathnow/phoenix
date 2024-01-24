/** @file pwdrules.h
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
#ifndef PWDRULES_H_
#define PWDRULES_H_

#include <string>
#include <vector>

namespace zios {
namespace common {

class PasswordRule {
public:
    PasswordRule();
    virtual ~PasswordRule();

    virtual bool isValidPassword(const std::string& password) const = 0;
    virtual const std::string& description() const = 0;
};

class MustContainEnoughChactersRule : public PasswordRule {
public:
    MustContainEnoughChactersRule(int requiredCharCount);
    ~MustContainEnoughChactersRule();

    static const std::string ERROR_MSG;

    bool isValidPassword(const std::string& password) const;
    const std::string& description() const { return _description; }

private:
    int _requiredCharCount;
    std::string _description;
};

class PasswordRulesEngine {
public:
    /**
     * Constructs a PasswordRulesEngine with a specific set of rules.  This object
     * will take ownership of the individual PasswordRule object contained in the
     * vector.  The vector will be cleared.
     *
     * @param rules - A vector of pointers to PasswordRules
     */
    PasswordRulesEngine(std::vector<PasswordRule*>& rules);
    virtual ~PasswordRulesEngine();

    /**
     * Validates a password by running it through all rules.
     *
     * @return true the password is valid, false if not
     */
    virtual bool isValidPassword(const std::string& password) const;

    /**
     * Returns a list of the rule descriptions that make up this engine.
     *
     * @param returnedDescriptions - Vector to receive the descriptions
     *
     * @return int - A count of the number of descriptions
     */
    virtual int descriptions(std::vector<std::string>& returnedDescriptions) const;


    /**
     * Factory method to create a new PasswordRulesEngine.
     *
     * @return PasswordRulesEngine* - Pointer to a new PasswordRulesEngine.  This is
     * a transfer of ownership.
     */
    static PasswordRulesEngine* newPasswordRulesEngine();

protected:
    PasswordRulesEngine();

private:
    std::vector<PasswordRule*> _passwordRules;
};

} /* namespace web */
} /* namespace zios */

#endif /* PWDRULES_H_ */
