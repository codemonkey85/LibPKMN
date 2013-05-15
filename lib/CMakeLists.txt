#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Place database location into relevant source files
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} PKMNSIM_PKG_DATA_PATH)
MESSAGE(STATUS "")

add_subdirectory(pokehack)
FILE(GLOB pokehack_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "pokehack/*.c*")

add_subdirectory(sqlitecpp)
FILE(GLOB sqlitecpp_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "sqlitecpp/*.c*")

#Configure database info for relevant library files
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/internal.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/internal.cpp
@ONLY)

LIST(APPEND libpkmnsim_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen1impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen2impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen3impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_nature.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen1impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen2impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen3impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/analysis.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_move.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_move_mainimpl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/internal.cpp
    ${pokehack_sources}
    ${sqlitecpp_sources}
)

FILE(GLOB pkmnsim_lib_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
FOREACH(header ${pkmnsim_lib_headers})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${header}
        ${CMAKE_CURRENT_BINARY_DIR}/${header}
    )
ENDFOREACH(header)

ADD_LIBRARY(pkmnsim SHARED ${libpkmnsim_sources})
TARGET_LINK_LIBRARIES(pkmnsim ${SQLITE3_LIBRARY} ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(pkmnsim PROPERTIES DEFINE_SYMBOL "PKMNSIM_DLL_EXPORTS")
INSTALL(TARGETS pkmnsim
    LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT libraries #.so
    ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT libraries #.lib
    RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT libraries #.dll
)
