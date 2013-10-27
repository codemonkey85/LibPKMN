#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Extract Git commits from PKMNsim and all submodules

FIND_PACKAGE(Git)

IF(GIT_FOUND AND EXISTS ${PKMNSIM_SOURCE_DIR}/.git)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Extracting compilation info from Git and build dependencies...")
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE PKMNSIM_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}
    )
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE PKMNSIM_IMAGES_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}/share/pkmnsim/images
    )
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE POKEHACK_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}/pokehack
    )
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE POKELIB_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}/PokeLib
    )
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE PKMDS_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}/PKMDS-G5
    )
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE SQLITECPP_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}/lib/SQLiteCpp
    )
ELSE()
    SET(PKMNSIM_GIT_DESCRIBE "unknown commit")
    SET(PKMNSIM_IMAGES_GIT_DESCRIBE "unknown commit")
    SET(POKEHACK_GIT_DESCRIBE "unknown commit")
    SET(POKELIB_GIT_DESCRIBE "unknown commit")
    SET(PKMDS_GIT_DESCRIBE "unknown commit")
    SET(SQLITECPP_GIT_DESCRIBE "unknown commit")
ENDIF(GIT_FOUND AND EXISTS ${PKMNSIM_SOURCE_DIR}/.git)

#Make values persistent so this only needs to be run once
SET(PKMNSIM_GIT_DESCRIBE "${PKMNSIM_GIT_DESCRIBE}" CACHE STRING "Pokemon-sim git describe output")
SET(PKMNSIM_IMAGES_GIT_DESCRIBE "${PKMNSIM_IMAGES_GIT_DESCRIBE}" CACHE STRING "Pokemon-sim Images git describe output")
SET(POKEHACK_GIT_DESCRIBE "${POKEHACK_GIT_DESCRIBE}" CACHE STRING "Pokehack git describe output")
SET(POKELIB_GIT_DESCRIBE "${POKELIB_GIT_DESCRIBE}" CACHE STRING "PokeLib git describe output")
SET(PKMDS_GIT_DESCRIBE "${PKMDS_GIT_DESCRIBE}" CACHE STRING "PKMDS git describe output")
SET(SQLITECPP_GIT_DESCRIBE "${SQLITECPP_GIT_DESCRIBE}" CACHE STRING "SQLiteC++ git describe output")