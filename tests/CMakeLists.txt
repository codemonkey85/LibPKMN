#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

#Get images dir
SET(IMAGES_PATH ${LIBPKMN_BINARY_DIR}/share/libpkmn/images)
FILE(TO_NATIVE_PATH ${IMAGES_PATH} IMAGES_PATH)

#Get SQLite database dir
SET(DATABASE_PATH ${LIBPKMN_BINARY_DIR}/share/libpkmn)
FILE(TO_NATIVE_PATH ${DATABASE_PATH} DATABASE_PATH)

SET(cpp_test_sources
    dict_test.cpp
    pokemon_text_test.cpp
)

INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/lib/conversions)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/rpokesav/include)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/pokehack/include)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/PokeLib-NC/include)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/PKMDS/include)

MACRO(LIBPKMN_ADD_TEST test_name test_cmd)
    IF(WIN32)
        SET(TEST_CMD ${test_cmd})
        STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
        STRING(REPLACE "/" "\\" LIBPKMN_BINARY_DIR "${LIBPKMN_BINARY_DIR}")
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        SET(TEST_CMD ${test_cmd})
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_template.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
    ENDIF()
ENDMACRO(LIBPKMN_ADD_TEST)

MACRO(LIBPKMN_ADD_LINUX_CS_TEST test_name)
    SET(TEST_NAME ${test_name})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/cs_unit_test_template.sh.in
        ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
    @ONLY)
    ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
ENDMACRO(LIBPKMN_ADD_LINUX_CS_TEST)

##################################################
# C++ tests
##################################################
FOREACH(cpp_test ${cpp_test_sources})
    GET_FILENAME_COMPONENT(cpp_test_name ${cpp_test} NAME_WE)
    ADD_EXECUTABLE(${cpp_test_name} ${cpp_test})
    TARGET_LINK_LIBRARIES(${cpp_test_name} pkmn)

    IF(CMAKE_COMPILER_IS_GNUCXX)
        IF(UNIX)
            SET_TARGET_PROPERTIES(${cpp_test_name} PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
        ELSE() #Cygwin
            SET_TARGET_PROPERTIES(${cpp_test_name} PROPERTIES COMPILE_FLAGS "-std=c++0x")
        ENDIF(UNIX)
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(${cpp_test_name} PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

    IF(WIN32)
        LIBPKMN_ADD_TEST(${cpp_test_name} "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${cpp_test_name}.exe")
    ELSE()
        LIBPKMN_ADD_TEST(${cpp_test_name} "${CMAKE_CURRENT_BINARY_DIR}/${cpp_test_name}")
    ENDIF(WIN32)
ENDFOREACH(cpp_test ${cpp_test_sources})

##################################################
# Python tests
##################################################
IF(ENABLE_PYTHON)
    SET(python_test_names
        python_dict_test
        python_enum_test
        python_pkmn_array_test
        python_pokemon_text_test
    )

    FOREACH(python_test ${python_test_names})
        LIBPKMN_ADD_TEST(${python_test} "${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${python_test}.py")
    ENDFOREACH(python_test ${python_test_names})
ENDIF(ENABLE_PYTHON)

##################################################
# C# tests
##################################################
IF(ENABLE_CSHARP)
    FILE(TO_NATIVE_PATH "${LIBPKMN_BINARY_DIR}/lib/swig/cs/libpkmn-cs.dll" LIBPKMN_CS_DLL)
    SET(LIBPKMN_CS_DLL ${LIBPKMN_CS_DLL} CACHE FILEPATH "libpkmn-cs.dll location")
    FILE(TO_NATIVE_PATH "${LIBPKMN_BINARY_DIR}/lib/swig/cs" LIBRARY_PATH)
    SET(LIBRARY_PATH "${LIBRARY_PATH}" CACHE FILEPATH "LibPKMN C# library directory")

    SET(cs_test_names
        CSharpDictTest
        CSharpPokemonTextTest
    )

    FOREACH(cs_test ${cs_test_names})
        FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.cs)
        FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe)
        FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/libpkmn-cs.dll)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/${cs_test}.cs
            ${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.cs
        )
        ADD_CUSTOM_COMMAND(
            DEPENDS libpkmn-cs
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe
            COMMAND ${CSHARP_COMPILER} ARGS "/r:${LIBPKMN_CS_DLL}" "/platform:x86" "/out:${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe" "${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.cs"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Building C# executable ${cs_test}"
        )
        ADD_CUSTOM_TARGET(
            ${cs_test} ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe
        )
        IF(WIN32)
            LIBPKMN_ADD_TEST(${cs_test} "${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe")
        ELSE()
            LIBPKMN_ADD_LINUX_CS_TEST(${cs_test})
        ENDIF(WIN32)
    ENDFOREACH(cs_test ${cs_test_names})
ENDIF(ENABLE_CSHARP)
