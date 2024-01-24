/** @file SystemTagNameTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 31, 2018 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>

#include <SystemTagName.h>

namespace zios {
namespace common {


using namespace std;

class WhenSystemTagNameIsAskedIfIdIsSystemId : public testing::Test {
public:
    WhenSystemTagNameIsAskedIfIdIsSystemId() {}
    ~WhenSystemTagNameIsAskedIfIdIsSystemId() {}
};

TEST_F(WhenSystemTagNameIsAskedIfIdIsSystemId, shouldTellTheTruth) {
    EXPECT_FALSE(SystemTagName::isSystemIOPointId(1000000));
    EXPECT_TRUE(SystemTagName::isSystemIOPointId(SystemTagName::SysRocSrcGroup_ID));
}

}
}
