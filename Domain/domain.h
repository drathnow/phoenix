#ifndef __DOMAIN_H___
#define __DOMAIN_H___
#include <map>
#include <string>

namespace zios::domain
{

enum DataType
{
    UNKNOWN = 0,  //!< UNKNOWN type (error)
    DISCRETE = 1, //!< Boolean (0 FALSE, !0 TRUE (usually 1))
    INT8 = 2,     //!< 1 byte signed
    UINT8 = 3,   //!< 1 byte unsigned
    INT16 = 4,    //!< 2 bytes signed
    UINT16 = 5,  //!< 2 bytes unsigned
    INT32 = 6,     //!< 4 bytes signed
    UINT32 = 7,   //!< 4 bytes unsigned
    FLOAT32 = 8,    //!< single precision floating point
    FLOAT64 = 9,   //!< double precision floating point
    STRING = 10,  //!< ascii string - 2 bytes length followed by [n] bytes character array. (serialized)
    BLOB = 11,    //!< binary blob - 4 bytes length followed by [n] bytes (serialized)
    INT64 = 12,   //!< 8 bytes signed
    UINT64 = 13, //!< 8 bytes unsigned
};

typedef uint16_t    pollset_id_t;
typedef uint32_t    index_id_t;
typedef std::map<std::string, std::string> persistence_map_t;

}

#endif /* __DOMAIN_H___ */
