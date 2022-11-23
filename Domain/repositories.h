#pragma once

#ifndef __REPOSITORIES_H___
#define __REPOSITORIES_H___
#include <vector>

#include "Deadband.h"
#include "Measurement.h"

namespace dios::domain
{
    class IIOPointRepository
    {
    public:
        IIOPointRepository() = default;
        virtual ~IIOPointRepository() = default;

        virtual iopoint_id_t createIOPoint(IOPoint& ioPoint) = 0;
        virtual int updateIOPoint(const IOPoint& ioPoint) = 0;
        virtual int deleteIOPointWithOid(iopoint_id_t oid) = 0;
        virtual IOPoint* ioPointForOid(iopoint_id_t oid) = 0;
        virtual int ioPoints(std::vector<IOPoint*>& entityVector, int count, iopoint_id_t fromOid = 0) = 0;
    };

//    class IDeadbandRepository
//    {
//    public:
//        IDeadbandRepository() = default;
//        virtual ~IDeadbandRepository() = default;
//
//        virtual iopoint_id_t createDeadband(Deadband& deadband) = 0;
//        virtual int updateDeadband(const Deadband& deadband) = 0;
//        virtual int deleteDeadbandWithOid(deadband_id_t oid) = 0;
//        virtual IOPoint* deadbandForOid(deadband_id_t oid) = 0;
//        virtual int deadbands(std::vector<Deadband*>& entityVector, int count, iopoint_id_t fromOid = 0) = 0;
//    };

}


#endif /* __REPOSITORIES_H___ */
