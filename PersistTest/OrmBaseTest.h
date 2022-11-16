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
            _dbFile("/mnt/ramdisk/db/" + dbFilename),
            _persist(persist)
    {
    }
    virtual ~OrmBaseTest() = default;

    virtual void SetUp();
    virtual void TearDown();
    void close();
    bool tableExists(const std::string &tableName);

protected:
    File _dbFile;
    bool _persist;
    sqlite3 *_dbContext{ nullptr };

    bool executeCommandInContext(const char* command);
    int executeCommandInContext(const char *command, sqlite3_callback callback, void *arg = nullptr);
    int rowCountInTable(const char *tableName, const char *whereClause = nullptr);
};

}
#endif
