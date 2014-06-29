#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR}/libpkmn.db" DATABASE_PATH)
SET(LIBPKMN_DATABASE_PATH ${DATABASE_PATH} CACHE FILEPATH "Database path")

FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR}/images" IMAGES_DIR)
SET(LIBPKMN_IMAGES_DIR ${IMAGES_DIR} CACHE FILEPATH "Database directory")

####################################################################
# Database dump is distributed instead of a binary, so compile and
# install in correct location
####################################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/database_concat.py.in
    ${CMAKE_CURRENT_BINARY_DIR}/database_concat.py
@ONLY)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libpkmn_db_dump
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libpkmn_db_additions
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/database_concat.py
    COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db ".read ${CMAKE_CURRENT_BINARY_DIR}/libpkmn_db_dump"
    COMMENT "Compiling LibPKMN SQLite database"
)
ADD_CUSTOM_TARGET(
    libpkmn.db ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
)

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
    DESTINATION ${PKG_DATA_DIR}
    COMPONENT "Database"
)
ADD_SUBDIRECTORY(images)
