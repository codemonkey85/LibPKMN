#Place database location into base_pkmn.cpp, pkmn_nature.cpp, and pkmn_types.cpp
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} PKMNSIM_PKG_DATA_PATH)
FILE(TO_NATIVE_PATH ${PKMNSIM_PKG_DATA_PATH}/pkmnsim.db PKMNSIM_DB)
STRING(REPLACE "\\" "\\\\" PKMNSIM_PKG_DATA_PATH ${PKMNSIM_PKG_DATA_PATH})
MESSAGE(STATUS "Full package data directory: ${PKMNSIM_PKG_DATA_PATH}")
MESSAGE(STATUS "")

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen1impl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn_gen1impl.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen2impl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn_gen2impl.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen3impl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn_gen3impl.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_nature.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_nature.cpp
@ONLY)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_types.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_types.cpp
@ONLY)

LIST(APPEND libpkmnsim_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn_gen1impl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn_gen2impl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/base_pkmn_gen3impl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_nature.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn_types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen1impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen2impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen3impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/analysis.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Column.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Database.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Statement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/Transaction.cpp
)

FILE(GLOB pkmnsim_lib_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
FOREACH(header ${pkmnsim_lib_headers})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${header}
        ${CMAKE_CURRENT_BINARY_DIR}/${header}
    )
ENDFOREACH(header)

ADD_LIBRARY(pkmnsim SHARED ${libpkmnsim_sources})
TARGET_LINK_LIBRARIES(pkmnsim ${SQLITE3_LIBRARY})
INSTALL(TARGETS pkmnsim
    LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT libraries #.so
    ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT libraries #.lib
    RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT libraries #.dll
)
