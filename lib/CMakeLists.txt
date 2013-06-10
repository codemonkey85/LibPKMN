#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Place database location into relevant source files
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} PKMNSIM_PKG_DATA_PATH)

IF(ENABLE_LIBPKMNSIM)
    add_subdirectory(sqlitecpp)
    FILE(GLOB sqlitecpp_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "sqlitecpp/*.c*")

    #Configure database info for relevant library files
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/internal.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/internal.cpp
    @ONLY)

    LIST(APPEND libpkmnsim_libs ${SQLITE3_LIBRARY} ${Boost_LIBRARIES})

    LIST(APPEND libpkmnsim_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen1impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen2impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen3impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_nature.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lists.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen1impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen2impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen3impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/analysis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_move.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_move_mainimpl.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/internal.cpp
        ${sqlitecpp_sources}
    )

    #Add Qt4 files if appropriate
    IF(ENABLE_LIBPKMNSIM_QT4)
        add_subdirectory(qt4_classes)
        INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)

        INCLUDE(${QT_USE_FILE})
        LIST(APPEND libpkmnsim_libs ${QT_LIBRARIES})

        FILE(GLOB libpkmnsim_qt4_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qt4_classes/*.cpp")
        FILE(GLOB libpkmnsim_qt4_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qt4_classes/*.hpp")

        QT4_WRAP_CPP(MOCSrcs ${libpkmnsim_qt4_headers})

        LIST(APPEND libpkmnsim_sources ${libpkmnsim_qt4_sources} ${MOCSrcs})
    ENDIF(ENABLE_LIBPKMNSIM_QT4)

    FILE(GLOB pkmnsim_lib_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
    FOREACH(header ${pkmnsim_lib_headers})
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/${header}
            ${CMAKE_CURRENT_BINARY_DIR}/${header}
        )
    ENDFOREACH(header)

    ADD_LIBRARY(pkmnsim SHARED ${libpkmnsim_sources})
    TARGET_LINK_LIBRARIES(pkmnsim ${libpkmnsim_libs})
    SET_TARGET_PROPERTIES(pkmnsim PROPERTIES DEFINE_SYMBOL "PKMNSIM_DLL_EXPORTS")
    IF(MSVC AND QT4_FOUND)
        #Otherwise, a command line terminal would open along with QT apps
        SET_TARGET_PROPERTIES(pkmnsim PROPERTIES LINK_FLAG_RELEASE "/SUBSYSTEM:WINDOWS")
    ENDIF(MSVC AND QT4_FOUND)
    INSTALL(TARGETS pkmnsim
        LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.so
        ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.lib
        RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Libraries #.dll
    )
ENDIF(ENABLE_LIBPKMNSIM)
