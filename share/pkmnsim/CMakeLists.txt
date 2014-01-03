#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR}" DATABASE_DIR)
SET(PKMNSIM_DATABASE_DIR ${DATABASE_DIR} CACHE FILEPATH "Database directory")

FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR}/images" IMAGES_DIR)
SET(PKMNSIM_IMAGES_DIR ${IMAGES_DIR} CACHE FILEPATH "Database directory")

FILE(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db")

####################################################################
# Database dump is distributed instead of a binary, so compile and
# install in correct location
####################################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/libpkmn_db_dump
    ${CMAKE_CURRENT_BINARY_DIR}/libpkmn_db_dump
COPYPONLY)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
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
