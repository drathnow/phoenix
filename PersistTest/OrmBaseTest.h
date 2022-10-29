#pragma once

#ifndef __ORMBASETEST_H__
#define __ORMBASETEST_H__

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sqlite3.h>
#include <iostream>
#include <file.h>

#include <DbUpgrader.h>

namespace dios::persist
{

using namespace std;
using namespace dios::foundation;

class OrmBaseTest: public testing::Test
{
public:
    OrmBaseTest(string dbFilename, bool persist = false) :
            _dbFile("./db/" + dbFilename),
            _persist(persist)
    {
    }
    virtual ~OrmBaseTest() = default;

    void SetUp();
    void TearDown();
    void close();
    bool tableExists(const std::string &tableName);

protected:
    File _dbFile;
    bool _persist;
    sqlite3 *_dbContext{ nullptr };
};

}
#endif
