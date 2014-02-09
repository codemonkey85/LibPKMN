#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")

INCLUDE(LibPKMNPython)
PYTHON_BUILD_MODULE(libpkmn_python_swig LibPKMN)

ADD_SUBDIRECTORY(analysis)
ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(io)

##########################################################
# Copy manually generated files to binary directory
##########################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/Forms.py
    ${CMAKE_CURRENT_BINARY_DIR}/Forms.py
COPYONLY)
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/Ribbons.py
    ${CMAKE_CURRENT_BINARY_DIR}/Ribbons.py
COPYONLY)

##########################################################
# Generate Python enum classes
##########################################################
FILE(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/python_enums.cmake")
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/python_enums.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/python_enums.cmake
@ONLY)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Species.py
    COMMAND ${CMAKE_COMMAND} -DLIBPKMN_SOURCE_DIR=${LIBPKMN_SOURCE_DIR}
                             -DLIBPKMN_BINARY_DIR=${LIBPKMN_BINARY_DIR}
                             -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                             -DLIBPKMN_PYTHON_DIR=${LIBPKMN_PYTHON_DIR}
                             -P ${CMAKE_CURRENT_BINARY_DIR}/python_enums.cmake
    COMMENT "Generating Python enum classes"
)
ADD_CUSTOM_TARGET(
    Python_enums ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Species.py
)
#Install generated Python files
INSTALL(CODE
    "FILE(GLOB generated_files \"${LIBPKMN_BINARY_DIR}/lib/swig/python/*.py\")
    FOREACH(file \${generated_files})
        FILE(INSTALL DESTINATION \"${CMAKE_INSTALL_PREFIX}/${LIBPKMN_PYTHON_DIR}/LibPKMN\" TYPE FILE FILES \"\${file}\")
    ENDFOREACH(file \${generated_files})"
COMPONENT "LibPKMN_Python")
