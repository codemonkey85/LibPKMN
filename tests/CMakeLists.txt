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
SET(DATABASE_PATH ${LIBPKMN_BINARY_DIR}/share/libpkmn/libpkmn.db)
FILE(TO_NATIVE_PATH ${DATABASE_PATH} DATABASE_PATH)

SET(cpp_test_sources
    db_additions_test.cpp
    dict_test.cpp
    pokemon_text_test.cpp
    bag_pocket_test.cpp
    prng_test.cpp
    copy_sptr_test.cpp
)

INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/lib/conversions)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/rpokesav/include)
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
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_TEST)

MACRO(LIBPKMN_ADD_PYTHON_TEST test_name)
    SET(TEST_CMD "${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${python_test}.py")
    FILE(TO_NATIVE_PATH "${LIBPKMN_BINARY_DIR}/lib/swig" LIBPKMN_SWIG_DIR)
    SET(LIBPKMN_SWIG_DIR ${LIBPKMN_SWIG_DIR} CACHE FILEPATH "LibPKMN SWIG build directory")
    SET(PYTHON_PATH "${LIBPKMN_SWIG_DIR}")
    IF(WIN32)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/python_unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        LIBPKMN_ADD_TEST(${python_test} "${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${python_test}.py")
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_PYTHON_TEST)

MACRO(LIBPKMN_ADD_CS_TEST test_name)
    SET(TEST_NAME ${test_name})
    IF(WIN32)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/cs_unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/cs_unit_test_template.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_CS_TEST)

MACRO(LIBPKMN_ADD_JAVA_TEST test_name)
    SET(TEST_NAME ${test_name})
    IF(WIN32)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/java_unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/java_unit_test_template.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_JAVA_TEST)

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
        python_pokemon_text_test
        python_bag_pocket_test
        python_copy_sptr_test
    )

    FOREACH(python_test ${python_test_names})
        LIBPKMN_ADD_PYTHON_TEST(${python_test})
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
        CSharpBagPocketTest
        CSharpCopySPtrTest
    )

    FOREACH(cs_test ${cs_test_names})
        ADD_CUSTOM_COMMAND(
            DEPENDS libpkmn-cs
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${cs_test}.cs
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe
            COMMAND ${CSHARP_COMPILER} ARGS "/r:${LIBPKMN_CS_DLL}" "/platform:x86" "/out:${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe" "${CMAKE_CURRENT_SOURCE_DIR}/${cs_test}.cs"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Building C# executable ${cs_test}"
        )
        ADD_CUSTOM_TARGET(
            ${cs_test} ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${cs_test}.exe
        )
        LIBPKMN_ADD_CS_TEST(${cs_test})
    ENDFOREACH(cs_test ${cs_test_names})
ENDIF(ENABLE_CSHARP)

##################################################
# Java tests
##################################################
IF(ENABLE_JAVA)
    FILE(TO_NATIVE_PATH "${LIBPKMN_BINARY_DIR}/lib/swig/java/LibPKMN.jar" LIBPKMN_JAR)
    SET(LIBPKMN_JAR ${LIBPKMN_JAR} CACHE FILEPATH "LibPKMN.jar location")

    SET(java_test_names
        JavaDictTest
        JavaBagPocketTest
        JavaCopySPtrTest
    )

    FOREACH(java_test ${java_test_names})
        ADD_CUSTOM_COMMAND(
            DEPENDS LibPKMN_jar
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${java_test}.java
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${java_test}.class
            COMMAND ${Java_JAVAC_EXECUTABLE} -classpath ${LIBPKMN_JAR} -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${java_test}.java
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Building Java program ${java_test}"
        )
        ADD_CUSTOM_TARGET(
            ${java_test} ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${java_test}.class
        )
        LIBPKMN_ADD_JAVA_TEST(${java_test})
    ENDFOREACH(java_test ${java_test_names})
ENDIF(ENABLE_JAVA)
