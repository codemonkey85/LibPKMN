#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR}" DATABASE_DIR)
SET(DATABASE_DIR ${DATABASE_DIR} CACHE FILEPATH "Database directory")

FILE(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db")

#Copy over to binary directory for proper out-of-date behavior
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmnsim_db_dump
    ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim_db_dump
)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db
    COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db ".read ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim_db_dump"
    COMMENT "Compiling SQLite database"
)

ADD_CUSTOM_TARGET(
    pkmnsim.db ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db
)

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db
    DESTINATION ${PKG_DATA_DIR}
    COMPONENT "database"
)
ADD_SUBDIRECTORY(images)
