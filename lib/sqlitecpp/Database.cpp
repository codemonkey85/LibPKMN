/**
 * @file  Database.cpp
 * @brief Management of a SQLite Database Connection.
 *
 * Copyright (c) 2012 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 * Copyright (c) 2012 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <sqlitecpp/Database.h>

#include <sqlitecpp/Statement.h>


namespace SQLite
{

// Open the provided database UTF-8 filename with SQLITE_OPEN_xxx provided flags.
Database::Database(const char* apFilename, const int aFlags /*= SQLITE_OPEN_READONLY*/) : // throw(SQLite::Exception)
    mpSQLite(NULL),
    mFilename(apFilename)
{
    // TODO SRombauts : use the "zVfs" (last) parameter to give access to the ":memory:" VFS module
    // TODO SRombauts : then add a backup mode to/from ":memory:"
    int ret = sqlite3_open_v2(apFilename, &mpSQLite, aFlags, NULL);
    if (SQLITE_OK != ret)
    {
        std::string strerr = sqlite3_errmsg(mpSQLite);
        sqlite3_close(mpSQLite);
        throw SQLite::Exception(strerr);
    }
}

// Close the SQLite database connection.
Database::~Database(void) throw() // nothrow
{
    int ret = sqlite3_close(mpSQLite);
    if (SQLITE_OK != ret)
    {
        // Never throw an exception in a destructor
        //std::cout << sqlite3_errmsg(mpSQLite);
    }
}

// Shortcut to execute one or multiple SQL statements without results.
int Database::exec(const char* apQueries) // throw(SQLite::Exception);
{
    int ret = sqlite3_exec(mpSQLite, apQueries, NULL, NULL, NULL);
    check(ret);

    // Return the number of changes made by those SQL statements
    return sqlite3_changes(mpSQLite);
}

// Shortcut to execute a one step query and fetch the first column of the result.
// WARNING: Be very careful with this dangerous method: you have to
// make a COPY OF THE result, else it will be destroy before the next line
// (when the underlying temporary Statement and Column objects are destroyed)
// this is an issue only for pointer type result (ie. char* and blob)
Column Database::execAndGet(const char* apQuery) // throw(SQLite::Exception)
{
    Statement query(*this, apQuery);
    query.executeStep();
    return query.getColumn(0);
}

//execAndGet used to produce more detailed error messages
Column Database::execAndGet(const char* apQuery, std::string identifier) // throw(SQLite::Exception)
{
    Statement query(*this, apQuery);
    query.executeStep();
    return query.getColumn(0, identifier);
}

//execAndGet used to specifically get std::strings
std::string Database::execAndGetStr(const char* apQuery, std::string identifier) // throw(SQLite::Exception)
{
    Statement query(*this, apQuery);
    query.executeStep();
    std::string result = query.getColumn(0, identifier);
    return result;
}

// Shortcut to test if a table exists.
bool Database::tableExists(const char* apTableName) // throw(SQLite::Exception)
{
    Statement query(*this, "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?");
    query.bind(1, apTableName);
    query.executeStep();
    int Nb = query.getColumn(0);
    return (1 == Nb);
}

// Check if aRet equal SQLITE_OK, else throw a SQLite::Exception with the SQLite error message
void Database::check(const int aRet) const // throw(SQLite::Exception)
{
    if (SQLITE_OK != aRet)
    {
        throw SQLite::Exception(sqlite3_errmsg(mpSQLite));
    }
}

}  // namespace SQLite
