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
    MESSAGE(STATUS "Extracting compilation info for pkmnsim-config-info...")
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
ELSE()
    SET(PKMNSIM_GIT_DESCRIBE "unknown commit")
    SET(PKMNSIM_IMAGES_GIT_DESCRIBE "unknown commit")
    SET(POKEHACK_GIT_DESCRIBE "unknown commit")
    SET(POKELIB_GIT_DESCRIBE "unknown commit")
    SET(PKMDS_GIT_DESCRIBE "unknown commit")
ENDIF(GIT_FOUND AND EXISTS ${PKMNSIM_SOURCE_DIR}/.git)

EXECUTE_PROCESS(
    COMMAND ${SQLITE3_EXECUTABLE} -version
    OUTPUT_VARIABLE SQLITE3_RAW_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}
)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmnsim-config-info.cpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-config-info.cpp
@ONLY)
ADD_EXECUTABLE(pkmnsim-config-info ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-config-info.cpp)
TARGET_LINK_LIBRARIES(pkmnsim-config-info ${Boost_LIBRARIES} pkmnsim)
INSTALL(TARGETS pkmnsim-config-info RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
