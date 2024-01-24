/** @file HasherFactoryTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 21, 2014 by daver:
 * BugFix_ Jan 08, 2015 by eb:  add cJSON_Delete(_root) and delete hash
 * @endhistory
 */

#include <gtest/gtest.h>
#include <Hasher.h>

namespace zios {
namespace common_tests {

using namespace zios::common;

class HasherFactoryTest : public testing::Test {

};

const uint8_t* SHA1_CONFIG = (const uint8_t*)
        "{"                                 \
        "    \"HashType\": {"               \
        "        \"Name\" : \"SHA1\","      \
        "        \"IterationCount\":  1024" \
        "    }"                             \
        "}";


TEST_F(HasherFactoryTest, shouldReturnSHA1Hasher) {

    cJSON* root = cJSON_Parse((char*)SHA1_CONFIG);
    cJSON* item = cJSON_GetObjectItem(root, "HashType");

    Sha1Hasher* hash = (Sha1Hasher*)HashFactory::hasherForJsonConfig(item);

    ASSERT_TRUE(hash != NULL);
    ASSERT_EQ((uint32_t)1024, hash->iterations());
    cJSON_Delete(root);
    delete hash;

}

}
}

