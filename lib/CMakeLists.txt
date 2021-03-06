#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Place database location into relevant source files
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} LIBPKMN_PKG_DATA_PATH)

IF(ENABLE_LIBPKMN)
    ADD_SUBDIRECTORY(SQLiteCpp)
    ADD_SUBDIRECTORY(libspec)

    FILE(GLOB database_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "database/*.c*")
    
    #Build-time info
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/build_info.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/build_info.cpp
    @ONLY)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/paths.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
    @ONLY)

    INCLUDE(${QT_USE_FILE})
    LIST(APPEND libpkmn_libs ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
    LIST(APPEND libpkmn_libs ${Boost_LIBRARIES} rpokesav pokelib-nc pkmds SQLiteCpp spec)

    LIST(APPEND libpkmn_sources
        ${CMAKE_CURRENT_BINARY_DIR}/build_info.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
        library_bridge.cpp
        prng_impl.cpp
        pokemon_text.cpp
        nature.cpp
        calculations.cpp
        base_pokemon_impl.cpp
        base_pokemon_gen1impl.cpp
        base_pokemon_gen2impl.cpp
        base_pokemon_modernimpl.cpp
        lists.cpp
        team_pokemon_impl.cpp
        team_pokemon_gen1impl.cpp
        team_pokemon_gen2impl.cpp
        team_pokemon_modernimpl.cpp
        pocket_impl.cpp
        bag_impl.cpp
        item_impl.cpp
        item_machineimpl.cpp
        trainer_impl.cpp
        move_impl.cpp
        game_save_impl.cpp
        game_save_gen1impl.cpp
        game_save_gen3impl.cpp
        game_save_gen4impl.cpp
        game_save_gen5impl.cpp
        io.cpp
        copy_sptrs.cpp
        conversions/items.cpp
        conversions/pokemon.cpp
        conversions/trainer.cpp
        ${analysis_sources}
        ${conversions_sources}
        ${database_sources}
    )
    
    INCLUDE_DIRECTORIES(${LIBPKMN_BINARY_DIR}/include)

    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/qt4/AboutMessageBox.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/qt4/AboutMessageBox.cpp
    )
    
    FILE(GLOB libpkmn_qt4_headers "${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/*.hpp")
    FILE(GLOB libpkmn_qt4_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "qt4/*.cpp")
    LIST(APPEND libpkmn_qt4_sources ${CMAKE_CURRENT_BINARY_DIR}/qt4/AboutMessageBox.cpp)

    QT4_WRAP_CPP(MOCSrcs ${libpkmn_qt4_headers})

    LIST(APPEND libpkmn_sources ${libpkmn_qt4_sources} ${MOCSrcs})

    ####################################################################
    # Add DLL resource file when using MSVC
    ####################################################################
    IF(MSVC)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.rc.in
            ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc
        @ONLY)
        LIST(APPEND libpkmn_sources ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc)
    ENDIF(MSVC)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
    INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/rpokesav/include)
    INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/PokeLib-NC/include)
    INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/PKMDS/include)
    ADD_LIBRARY(pkmn SHARED ${libpkmn_sources})
    TARGET_LINK_LIBRARIES(pkmn ${libpkmn_libs})

    ####################################################################
    # Compiler-specific flags
    ####################################################################
    IF(CMAKE_COMPILER_IS_GNUCXX)
        IF(LINUX)
            SET_TARGET_PROPERTIES(pkmn PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
        ELSE() #Cygwin
            SET_TARGET_PROPERTIES(pkmn PROPERTIES COMPILE_FLAGS "-std=c++0x")
        ENDIF(LINUX)
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(pkmn PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

    ####################################################################
    # MSVC needs this setting to export functions in pkmn.dll
    ####################################################################
    SET_TARGET_PROPERTIES(pkmn PROPERTIES DEFINE_SYMBOL "PKMN_DLL_EXPORTS")
    INSTALL(TARGETS pkmn
        LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.so
        ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.lib
        RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime #.dll
    )
    IF(NOT DISABLE_SWIG)
        ADD_SUBDIRECTORY(swig)
    ENDIF(NOT DISABLE_SWIG)

    IF(UNIX)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.pc.in
            ${CMAKE_CURRENT_BINARY_DIR}/pkmn.pc
        @ONLY)

        INSTALL(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/pkmn.pc
            DESTINATION ${LIBRARY_DIR}/pkgconfig
            COMPONENT libraries
        )
    ENDIF(UNIX)
ENDIF(ENABLE_LIBPKMN)
