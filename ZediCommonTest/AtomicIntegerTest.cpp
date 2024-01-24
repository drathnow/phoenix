/** @file AtomicIntegerTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 22, 2013 by daver:
 * @endhistory
 */
#include "gtest/gtest.h"
#include "AtomicInteger.h"

using namespace zios::common;

class AtomicIntegerTest: public testing::Test {
};

TEST_F(AtomicIntegerTest, shouldSetInitialValue) {
    AtomicInteger ai(5);
    ASSERT_EQ(5, ai.get());
}

TEST_F(AtomicIntegerTest, shouldIncrementValue) {
    AtomicInteger ai(5);
    ASSERT_EQ(6, ai.incrementAndGet());
    ai.increment();
    ASSERT_EQ(7, ai.get());
}

TEST_F(AtomicIntegerTest, shouldDecrementValue) {
    AtomicInteger ai(5);
    ASSERT_EQ(4, ai.decrementAndGet());
    ai.decrement();
    ASSERT_EQ(3, ai.get());
}
