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
        IF(WIN32)
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            STRING(REPLACE "/" "\\" LIBPKMN_BINARY_DIR "${LIBPKMN_BINARY_DIR}")
        ENDIF(WIN32)
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
    SET(PYTHON_PATH ${LIBPKMN_BINARY_DIR}/lib/swig)

    SET(python_test_names
        python_dict_test
        python_enum_test
    )

    FOREACH(python_test ${python_test_names})
        LIBPKMN_ADD_TEST(${python_test} "${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${python_test}.py")
    ENDFOREACH(python_test ${python_test_names})
ENDIF(ENABLE_PYTHON)

