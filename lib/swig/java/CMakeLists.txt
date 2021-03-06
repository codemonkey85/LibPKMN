#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN)
SET(JAVA_INSTALL_DIRECTORY share/java)

#Generate enums
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN/Abilities.java
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=java --include-path=${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp --output-dir=${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Java enums"
)
ADD_CUSTOM_TARGET(
    java_enums ALL
    DEPENDS ${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN/Abilities.java
)

INCLUDE(UseSWIG)
INCLUDE(LibPKMNJava)

JAVA_BUILD_SWIG_MODULE(libpkmn_java_swig Core)
JAVA_BUILD_SWIG_MODULE(calculations_java_swig Calculations)
JAVA_BUILD_SWIG_MODULE(database_java_swig Database)
JAVA_BUILD_SWIG_MODULE(io_java_swig IO)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar
    COMMENT "Creating LibPKMN.jar"
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN/*.java
    COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar -C ${CMAKE_CURRENT_BINARY_DIR} org
    DEPENDS ${SWIG_MODULE_libpkmn_java_swig_REAL_NAME}
    DEPENDS ${SWIG_MODULE_calculations_java_swig_REAL_NAME}
    DEPENDS ${SWIG_MODULE_database_java_swig_REAL_NAME}
    DEPENDS ${SWIG_MODULE_io_java_swig_REAL_NAME}
    DEPENDS java_enums
)
ADD_CUSTOM_TARGET(
    LibPKMN_jar ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar
)
INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/LibPKMN.jar
    DESTINATION ${JAVA_INSTALL_DIRECTORY}
    COMPONENT "LibPKMN_Java"
)
