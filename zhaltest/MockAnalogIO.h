#ifndef MOCKANALOGIO_H__
#define MOCKANALOGIO_H__

#include <gmock/gmock.h>
#include <zhal.h>
#include <IAnalogIO.h>

namespace zios {
namespace hal {

class MockAnalogIO : public IAnalogIO
{
public:
    MockAnalogIO() : IAnalogIO(IHardwareIO::AI_T) {}
   ~MockAnalogIO(){}

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

    MOCK_METHOD1(setSlope, void(float slope));
    MOCK_METHOD1(setOffset, void(float offset));
    MOCK_METHOD0(getReferenceFor4to20, IDigitalIO*());
    MOCK_CONST_METHOD0(slopeID, iopoint_id_t());
    MOCK_METHOD1(setSlopeID, void(iopoint_id_t ioId));
    MOCK_CONST_METHOD0(offsetID, uint32_t());
    MOCK_METHOD1(setOffsetID, void(iopoint_id_t ofsetId));
    MOCK_METHOD0(convert, int32_t());
};

}
}  // namespace zios

#endif
