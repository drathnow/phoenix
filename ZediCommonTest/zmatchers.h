/** @file zmatchers.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 12, 2016 by daver:
 * @endhistory
 */
#ifndef ZMATCHERS_H_
#define ZMATCHERS_H_

#include <gmock/gmock-matchers.h>

using testing::MatcherInterface;
using testing::MatchResultListener;
using testing::Matcher;

class FloatApproxMatcher: public MatcherInterface<float> {
public:
    explicit FloatApproxMatcher(float f, float precision) : _theFloat(f), _precision(precision) {
    }

    virtual bool MatchAndExplain(float aValue, MatchResultListener* listener) const {
        return abs(aValue - _theFloat) <= _precision;
    }

    virtual void DescribeTo(::std::ostream* os) const {
        *os << "Equals " << _theFloat << " within " << _precision;
    }

    virtual void DescribeNegationTo(::std::ostream* os) const {
        *os << "Is NOT equal to " << _theFloat << " within " << _precision;
    }

private:
    float _theFloat;
    float _precision;
};

inline Matcher<float> FloatApprox(float value, float precision) {
    return MakeMatcher(new FloatApproxMatcher(value, precision));
}
#endif /* ZMATCHERS_H_ */
