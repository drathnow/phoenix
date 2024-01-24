/** @file NotificationMatcher.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 20, 2016 by daver:
 * @endhistory
 */
#ifndef NOTIFICATIONMATCHER_H_
#define NOTIFICATIONMATCHER_H_
#include <gmock/gmock-matchers.h>

#include "Notification.h"

namespace zios {
namespace common {

using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::MakeMatcher;
using ::testing::Matcher;

class NotificationMatcher : public MatcherInterface<Notification*> {
public:
    explicit NotificationMatcher(Notification* notification) :
        _notification(notification) {
    }

    virtual bool MatchAndExplain(Notification* notification, MatchResultListener* listener) const {
        if (notification->name() != _notification->name())
            return false;
        if (notification->properties().size() != _notification->properties().size())
            return false;
        if (notification->tag() != _notification->tag())
            return false;
        for (std::map<string, string>::const_iterator iter = notification->properties().begin();
                    iter != notification->properties().end();
                        ++iter) {
            std::string key = iter->first;
            std::map<string, string>::const_iterator otherIter = _notification->properties().find(key);
            if (otherIter == _notification->properties().end())
                return false;
            if (iter->second != otherIter->second)
                return false;
        }
        return true;
    }

    virtual void DescribeTo(::std::ostream* os) const {
        *os << "Expected the right timval but it was wrong";
    }

    virtual void DescribeNegationTo(::std::ostream* os) const {
        *os << "Expected the right timval but it was wrong";
    }

private:
    Notification* _notification;
};

inline Matcher<Notification*> notificationMatching(Notification* notification) {
    return MakeMatcher(new NotificationMatcher(notification));
}

}
}


#endif /* NOTIFICATIONMATCHER_H_ */
