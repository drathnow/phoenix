#include <string>

#include "IOPointRepository.h"

namespace dios::persist
{
using namespace std;

IOPointRepository::IOPointRepository(sqlite3* dbContext) : Repository(dbContext)
{
}

IOPointRepository::~IOPointRepository()
{
}

iopoint_id_t IOPointRepository::insertIoPoint(const io_point_t& ioPoint)
{
    string sql = ioPoint.insert_statement();
    return 0;
}

io_point_t IOPointRepository::ioPointForIoPointId(iopoint_id_t ioPointId)
{

    return 0;
}


}
