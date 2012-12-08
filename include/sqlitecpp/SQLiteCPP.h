/**
 * @file  SQLiteCPP.h
 * @brief SQLiteCPP is a smart and simple CPP SQLite3 wrapper. This file is only "easy include" for other files.
 *
 * Copyright (c) 2012 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 * Copyright (c) 2012 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef SQLITECPP_H
#define SQLITECPP_H
#pragma once


// SQLiteCPP.h requires sqlite3, and the corresponding library header
#include "sqlite3.h"

// Include useful headers of SQLiteCPP
#include "Database.h"
#include "Statement.h"
#include "Column.h"
#include "Transaction.h"


/**
 * @brief Version numbers for SQLiteCPP are provided in the same way as sqlite3.h
 *
 * The [SQLITECPP_VERSION] C preprocessor macro in the SQLiteCPP.h header
 * evaluates to a string literal that is the SQLite version in the
 * format "X.Y.Z" where X is the major version number
 * and Y is the minor version number and Z is the release number.
 *
 * The [SQLITECPP_VERSION_NUMBER] C preprocessor macro resolves to an integer
 * with the value (X*1000000 + Y*1000 + Z) where X, Y, and Z are the same
 * numbers used in [SQLITECPP_VERSION].
 */
#define SQLITECPP_VERSION           "0.4.0"
#define SQLITECPP_VERSION_NUMBER    0004000

#endif /*SQLITECPP_H*/
