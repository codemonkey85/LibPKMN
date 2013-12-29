#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/lib/swig" PKMNSIM_SWIG_SOURCE_DIR)
SET(PKMNSIM_SWIG_SOURCE_DIR ${PKMNSIM_SWIG_SOURCE_DIR} CACHE FILEPATH "PKMNsim SWIG source directory")

INCLUDE(PKMNsimPython)
PYTHON_BUILD_MODULE(pypkmnsim_swig PyPKMNsim)

ADD_SUBDIRECTORY(analysis)
ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(io)

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
    COMMAND ${CMAKE_COMMAND} -DPKMNSIM_SOURCE_DIR=${PKMNSIM_SOURCE_DIR}
                             -DPKMNSIM_BINARY_DIR=${PKMNSIM_BINARY_DIR}
                             -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                             -DPKMNSIM_PYTHON_DIR=${PKMNSIM_PYTHON_DIR}
                             -P ${CMAKE_CURRENT_BINARY_DIR}/python_enums.cmake
    COMMENT "Generating PyPKMNsim enum classes"
)
ADD_CUSTOM_TARGET(
    PyPKMNsim_enums ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Species.py
)
#Install generated Python files
INSTALL(CODE
    "FILE(GLOB generated_files \"${PKMNSIM_BINARY_DIR}/lib/swig/python/*.py\")
    FOREACH(file \${generated_files})
        FILE(INSTALL DESTINATION \"${CMAKE_INSTALL_PREFIX}/${PKMNSIM_PYTHON_DIR}/PyPKMNsim\" TYPE FILE FILES \"\${file}\")
    ENDFOREACH(file \${generated_files})"
COMPONENT "PyPKMNsim")
