#Place database location into base_pkmn.cpp, pkmn_natures.cpp, and pkmn_types.cpp
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} PKMNSIM_PKG_DATA_PATH)
STRING(REPLACE "\\" "\\\\" PKMNSIM_PKG_DATA_PATH ${PKMNSIM_PKG_DATA_PATH})
MESSAGE(STATUS "Full package data directory: ${PKMNSIM_PKG_DATA_PATH}")
MESSAGE(STATUS "")

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/base_gen1pkmn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_gen1pkmn.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/base_gen2pkmn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_gen2pkmn.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_natures.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_natures.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_types.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_types.cpp
@ONLY)

LIST(APPEND libpkmnsim_sources
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_gen1pkmn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_gen2pkmn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_natures.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_gen1pkmn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_gen2pkmn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stat_analysis.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Column.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Database.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Statement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Transaction.cpp
)


ADD_LIBRARY(pkmnsim SHARED ${libpkmnsim_sources})
INSTALL(TARGETS pkmnsim
    LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT libraries #.so
    ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT libraries #.lib
    RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT libraries #.dll
)
