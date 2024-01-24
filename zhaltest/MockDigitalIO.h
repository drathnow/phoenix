#ifndef MOCKDIGITALIO_H__
#define MOCKDIGITALIO_H__

#include <gmock/gmock.h>
#include <IDigitalIO.h>

#include "MockHardwareIO.h"

namespace zios {
namespace hal {

class MockDigitalIO : public IDigitalIO {
public:
    MockDigitalIO(IHardwareIO::PORTTYPE portType) : IDigitalIO(portType) {}
    MockDigitalIO() : IDigitalIO(IHardwareIO::DI_T) {}
    ~MockDigitalIO() {}

    MOCK_CONST_METHOD0(name, const char*());
    MOCK_CONST_METHOD0(label, const char*());
    MOCK_CONST_METHOD0(displayOrder, uint32_t());
    MOCK_CONST_METHOD0(type, PORTTYPE());
    MOCK_METHOD1(updateDisplayOrder, void(uint32_t order));
    MOCK_METHOD0(getInt, uint32_t());
    MOCK_METHOD0(getFloat, float());
    MOCK_CONST_METHOD0(getResponseType, IHardwareIO::DATATYPE());
    MOCK_CONST_METHOD0(timestamp, long_timestamp_t());
    MOCK_CONST_METHOD0(isReadOnly, bool());
    MOCK_METHOD1(writeBool, bool(bool value));
    MOCK_METHOD0(getReferenceForScan, IDigitalIO*());
    MOCK_METHOD0(forceUpdate, void());
    MOCK_METHOD0(forceRead, void());
};

}
}

#endif
