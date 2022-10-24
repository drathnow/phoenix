
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <SourceAddress.h>

namespace dios::domain {

using namespace std;

TEST(WhenSourceAddressIsAskedForSourceAddressForStringForDigitalInternalAddressString, shouldReturnInteralAddressObject) {
    SourceAddress* sourceAddress = SourceAddress::sourceAddressForString("DI.");
    ASSERT_TRUE(NULL != sourceAddress);
    ASSERT_TRUE(sourceAddress->isInternal());
    delete sourceAddress;
}

TEST(WhenSourceAddressIsAskedForSourceAddressForStringForAnaloglInternalAddressString, shouldReturnInteralAddressObject) {
    SourceAddress* sourceAddress = SourceAddress::sourceAddressForString("AI.");
    ASSERT_TRUE(NULL != sourceAddress);
    ASSERT_TRUE(sourceAddress->isInternal());
    delete sourceAddress;
}

TEST(WhenSourceAddressIsAskedForSourceAddressForStringForRTDInternalAddressString, shouldReturnInteralAddressObject) {
    SourceAddress* sourceAddress = SourceAddress::sourceAddressForString("RTD.");
    ASSERT_TRUE(NULL != sourceAddress);
    ASSERT_TRUE(sourceAddress->isInternal());
    delete sourceAddress;
}

TEST(WhenSourceAddressIsAskedForSourceAddressForStringForExternalAddressString, shouldReturnInteralAddressObject) {
    SourceAddress* sourceAddress = SourceAddress::sourceAddressForString("EXT;mq;gb;ab:cd:ef:gh:ij:kl:01:02:03;DI.1");
    ASSERT_TRUE(NULL != sourceAddress);
    ASSERT_TRUE(sourceAddress->isExternal());
    delete sourceAddress;
}

/**
 * Note: This is an incomplete test. It is only here to keep other existing test happy until the RTU source
 * address is fully implemented.
 */
TEST(WhenSourceAddressIsAskedForSourceAddressForStringForRtuAddressString, shouldReturnRtuAddressObject) {
    SourceAddress* sourceAddress = SourceAddress::sourceAddressForString("MBS;40001;");
    ASSERT_TRUE(NULL != sourceAddress);
    ASSERT_TRUE(sourceAddress->isModbus());
    delete sourceAddress;
}

}
