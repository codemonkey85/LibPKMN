#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Place database location into relevant source files
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} PKMNSIM_PKG_DATA_PATH)

IF(ENABLE_LIBPKMNSIM)
    FILE(GLOB sqlitecpp_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "sqlitecpp/*.c*")
    FILE(GLOB database_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "database/*.c*")

    #Configure database info for relevant library files
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/paths.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
    @ONLY)

    LIST(APPEND libpkmnsim_libs ${Boost_LIBRARIES} pokehack pokelib pkmds)

    LIST(APPEND libpkmnsim_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen1impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen2impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_pkmn_gen345impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_nature.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmn_types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lists.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen1impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen2impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spec_pkmn_gen345impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/analysis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/trainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/trainer_gen3impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/trainer_gen4impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/trainer_gen5impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_move.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base_move_mainimpl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/conversions.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
        ${sqlitecpp_sources}
        ${database_sources}
    )
    
    ####################################################################
    # If Qt is found, add Qt subclasses to LibPKMNsim
    ####################################################################
    IF(ENABLE_LIBPKMNSIM_QT4)
        ADD_SUBDIRECTORY(qt4)
        INCLUDE_DIRECTORIES(${PKMNSIM_BINARY_DIR}/include)

        INCLUDE(${QT_USE_FILE})
        LIST(APPEND libpkmnsim_libs ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})

        FILE(GLOB libpkmnsim_qt4_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qt4/*.cpp")
        FILE(GLOB libpkmnsim_qt4_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qt4/*.hpp")

        QT4_WRAP_CPP(MOCSrcs ${libpkmnsim_qt4_headers})

        LIST(APPEND libpkmnsim_sources ${libpkmnsim_qt4_sources} ${MOCSrcs})
    ENDIF(ENABLE_LIBPKMNSIM_QT4)

    INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/pokehack/include)
    INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/PokeLib/include)
    INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/PKMDS-G5/include)
    ADD_LIBRARY(pkmnsim SHARED ${libpkmnsim_sources})
    TARGET_LINK_LIBRARIES(pkmnsim ${libpkmnsim_libs})

    ####################################################################
    # Compiler-specific flags
    ####################################################################
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_TARGET_PROPERTIES(pkmnsim PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(pkmnsim PROPERTIES COMPILE_FLAGS "-std=c++11")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

    ####################################################################
    # MSVC needs this setting to export functions in pkmnsim.dll
    ####################################################################
    SET_TARGET_PROPERTIES(pkmnsim PROPERTIES DEFINE_SYMBOL "PKMNSIM_DLL_EXPORTS")
    INSTALL(TARGETS pkmnsim
        LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.so
        ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.lib
        RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime #.dll
    )

    ADD_SUBDIRECTORY(swig)
ENDIF(ENABLE_LIBPKMNSIM)
