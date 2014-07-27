#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")

SET(LIBPKMN_JAVA_INCLUDE_DIRS
    ${LIBPKMN_SOURCE_DIR}/include
    ${LIBPKMN_SWIG_SOURCE_DIR}
    ${JAVA_INCLUDE_PATH}
    ${JNI_INCLUDE_DIRS}
)

SET(LIBPKMN_JAVA_LIBRARIES
    pkmn
)

INCLUDE_DIRECTORIES(${LIBPKMN_JAVA_INCLUDE_DIRS})
SET(JAVA_INSTALL_DIRECTORY share/java)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN)

#Generate enums
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Abilities.java
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=java --include-path=${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp --output-dir=${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Java enums"
)
ADD_CUSTOM_TARGET(
    java_enums ALL
    DEPENDS ${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Abilities.java
    DEPENDS libpkmn.db
)

INCLUDE(UseSWIG)
MACRO(JAVA_GENERATE_FILES swig_source java_module_name depend_module_name do_build)
    FOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})
    SET_SOURCE_FILES_PROPERTIES(${swig_source}.i PROPERTIES CPLUSPLUS ON)

    SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN)
    SET(CMAKE_SWIG_FLAGS -module ${java_module_name} -package "org.nc" ${CMAKE_GLOBAL_SWIG_FLAGS})
    SWIG_ADD_MODULE(${swig_source} java ${swig_source}.i)
    IF(NOT ${depend_module_name} STREQUAL "none")
        ADD_DEPENDENCIES(${SWIG_MODULE_${swig_source}_REAL_NAME} ${SWIG_MODULE_${depend_module_name}_REAL_NAME})
    ENDIF(NOT ${depend_module_name} STREQUAL "none")
    IF(UNIX)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "lib")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".so")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ELSE()
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".dll")
    ENDIF(UNIX)
    SWIG_LINK_LIBRARIES(${swig_source} ${LIBPKMN_JAVA_LIBRARIES})

    IF(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION ${RUNTIME_DIR}
            COMPONENT "LibPKMN_Java"
        )
    ELSE()
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION ${LIBRARY_DIR}
            COMPONENT "LibPKMN_Java"
        )
    ENDIF(WIN32)

    IF(${do_build} STREQUAL "true")
        ADD_CUSTOM_COMMAND(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar
            COMMENT "Creating LibPKMN.jar"
            COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN/*.java
            COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar -C ${CMAKE_CURRENT_BINARY_DIR} org
            DEPENDS ${SWIG_MODULE_${swig_source}_REAL_NAME}
        )
        ADD_CUSTOM_TARGET(
            LibPKMN_jar ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar
            DEPENDS java_enums
        )
        INSTALL(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar
            DESTINATION ${JAVA_INSTALL_DIRECTORY}
            COMPONENT "LibPKMN_Java"
        )
    ENDIF(${do_build} STREQUAL "true")
ENDMACRO(JAVA_GENERATE_FILES)

JAVA_GENERATE_FILES(libpkmn_java_swig Core none false)
JAVA_GENERATE_FILES(calculations_java_swig Calculations libpkmn_java_swig false)
JAVA_GENERATE_FILES(database_java_swig Database calculations_java_swig false)
JAVA_GENERATE_FILES(io_java_swig IO database_java_swig true)
