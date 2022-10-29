#pragma once

#ifndef __ENTITYREPOSITORY_H___
#define __ENTITYREPOSITORY_H___

#include <sqlite3.h>

namespace dios::persist
{

template<class D, class E>
class EntityRepository
{
public:
    EntityRepository(sqlite3* context) : _context {}
    ~EntityRepository() = default;

    bool insertEntity(E entity)
    {
        D dto;
    }
private:
    sqlite3* _context;
};

}
#endif /* __ENTITYREPOSITORY_H___ */
