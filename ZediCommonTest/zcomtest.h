/** @file zcomtest.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 15, 2016 by daver:
 * @endhistory
 */
#ifndef ZCOMTEST_H_
#define ZCOMTEST_H_

#include <stdint.h>

union int16_t_value_t {
    int16_t value;
    struct {
        char byte0;
        char byte1;
    } svar;
};

union uint16_t_value_t {
    uint16_t value;
    struct {
        char byte0;
        char byte1;
    } svar;
};

union int32_t_value_t {
    int32_t value;
    struct {
        char byte0;
        char byte1;
        char byte2;
        char byte3;
    } svar;
};

union uint32_t_value_t {
    uint32_t value;
    struct {
        char byte0;
        char byte1;
        char byte2;
        char byte3;
    } svar;
};

union float_value_t {
    float value;
    uint32_t uint32_tValue;
    struct {
        char byte0;
        char byte1;
        char byte2;
        char byte3;
    } svar;
};

union int64_t_value_t {
    int64_t value;
    struct {
        char byte0;
        char byte1;
        char byte2;
        char byte3;
        char byte4;
        char byte5;
        char byte6;
        char byte7;
    } svar;
};

union uint64_t_value_t {
    uint64_t value;
    struct {
        char byte0;
        char byte1;
        char byte2;
        char byte3;
        char byte4;
        char byte5;
        char byte6;
        char byte7;
    } svar;
};




#endif /* ZCOMTEST_H_ */
