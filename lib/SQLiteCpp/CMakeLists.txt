#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "Building SQLiteCpp as part of LibPKMN.")
MESSAGE(STATUS "")

SET(SQLiteCpp_sources
    sqlite3.c
    Column.cpp
    Database.cpp
    Statement.cpp
    Transaction.cpp
)
ADD_LIBRARY(SQLiteCpp STATIC ${SQLiteCpp_sources})
IF(UNIX)
    SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF(UNIX)
