#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Place database location into relevant source files
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} PKMNSIM_PKG_DATA_PATH)

IF(ENABLE_LIBRARY)
    ADD_SUBDIRECTORY(SQLiteCpp/sqlite3)
    INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/lib/SQLiteCpp/sqlite3)
    ADD_SUBDIRECTORY(SQLiteCpp/src)

    FILE(GLOB analysis_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "analysis/*.c*")
    FILE(GLOB database_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "database/*.c*")
    
    #Configure database info for relevant library files
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/paths.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
    @ONLY)

    INCLUDE(${QT_USE_FILE})
    LIST(APPEND libpkmnsim_libs ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
    LIST(APPEND libpkmnsim_libs ${Boost_LIBRARIES} pokehack pokelib-nc pkmds SQLiteCpp)

    LIST(APPEND libpkmnsim_sources
        library_bridge.cpp
        prng.cpp
        pokemon_text.cpp
        base_pokemon_impl.cpp
        base_pokemon_gen1impl.cpp
        base_pokemon_gen2impl.cpp
        base_pokemon_gen345impl.cpp
        lists.cpp
        team_pokemon_impl.cpp
        team_pokemon_gen1impl.cpp
        team_pokemon_gen2impl.cpp
        team_pokemon_gen345impl.cpp
        trainer_impl.cpp
        move_impl.cpp
        move_mainimpl.cpp
        #game_save_impl.cpp
        conversions/pokemon.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
        ${analysis_sources}
        ${conversions_sources}
        ${database_sources}
    )
    
    ####################################################################
    # If built standalone, build Qt4 subclasses
    ####################################################################
    IF(NOT PKMNSIM_USED_AS_SUBMODULE)
        ADD_SUBDIRECTORY(qt4)
        INCLUDE_DIRECTORIES(${PKMNSIM_BINARY_DIR}/include)
        
        FILE(GLOB libpkmnsim_qt4_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qt4/*.cpp")
        FILE(GLOB libpkmnsim_qt4_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qt4/*.hpp")
    
        QT4_WRAP_CPP(MOCSrcs ${libpkmnsim_qt4_headers})
    
        LIST(APPEND libpkmnsim_sources ${libpkmnsim_qt4_sources} ${MOCSrcs})
    ENDIF(NOT PKMNSIM_USED_AS_SUBMODULE)

    INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/pokehack/include)
    INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/PokeLib/include)
    INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/PKMDS-G5/include)
    ADD_LIBRARY(pkmnsim SHARED ${libpkmnsim_sources})
    TARGET_LINK_LIBRARIES(pkmnsim ${libpkmnsim_libs})

    ####################################################################
    # Compiler-specific flags
    ####################################################################
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_TARGET_PROPERTIES(pkmnsim PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(pkmnsim PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
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
ENDIF(ENABLE_LIBRARY)
