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
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(io)

FOREACH(namespace ${NAMESPACES})
    GENERATE_PYTHON_ENUM(${namespace})
ENDFOREACH(namespace ${NAMESPACES})

#Install generated Python files
FILE(GLOB py_files "${CMAKE_CURRENT_BINARY_DIR}/*.py")
INSTALL(
    FILES ${py_files}
    DESTINATION ${PKMNSIM_PYTHON_DIR}/PyPKMNsim
    COMPONENT "PyPKMNsim"
)