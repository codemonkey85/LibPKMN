#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${PKMNSIM_SOURCE_DIR}/lib/swig/pokelib" POKELIB_SWIG_SOURCE_DIR)
SET(POKELIB_SWIG_SOURCE_DIR ${POKELIB_SWIG_SOURCE_DIR} CACHE FILEPATH "PKMNsim SWIG source directory")

execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "
from distutils import sysconfig
print sysconfig.get_python_lib(plat_specific=True, prefix='')
" OUTPUT_VARIABLE POKELIB_PYTHON_DIR OUTPUT_STRIP_TRAILING_WHITESPACE
)
file(TO_CMAKE_PATH ${POKELIB_PYTHON_DIR} POKELIB_PYTHON_DIR)
SET(POKELIB_PYTHON_DIR ${POKELIB_PYTHON_DIR} CACHE FILEPATH "Python install directory")

INCLUDE(${SWIG_USE_FILE})

SET(POKELIB_PYTHON_INCLUDE_DIRS
    ${PKMNSIM_SOURCE_DIR}/include
    ${POKELIB_SWIG_SOURCE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)
INCLUDE_DIRECTORIES(${POKELIB_PYTHON_INCLUDE_DIRS})

SET(CMAKE_SWIG_FLAGS -module pypokelib_swig)
FOREACH(dir ${POKELIB_PYTHON_INCLUDE_DIRS})
    LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
ENDFOREACH(dir ${POKELIB_PYTHON_INCLUDE_DIRS})

SET(POKELIB_PYTHON_LIBRARIES
    pkmnsim
    pokelib
    ${PYTHON_LIBRARIES}
)

SET_SOURCE_FILES_PROPERTIES(pypokelib_swig.i PROPERTIES CPLUSPLUS ON)

####################################################################
# Create SWIG Python wrapper of PokeLib
####################################################################
INCLUDE(UseSWIG)
SWIG_ADD_MODULE(pypokelib_swig python pypokelib_swig.i)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_TARGET_PROPERTIES(${SWIG_MODULE_pypokelib_swig_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET_TARGET_PROPERTIES(${SWIG_MODULE_pypokelib_swig_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
SWIG_LINK_LIBRARIES(pypokelib_swig ${POKELIB_PYTHON_LIBRARIES})

SET(PYPOKELIB_REAL_SWIG_NAME ${SWIG_MODULE_pypokelib_swig_REAL_NAME} CACHE STRING "Real SWIG module name")

FILE(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/__init__.py")
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
    ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
COPYONLY)

LIST(APPEND py_files
    ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
    ${CMAKE_CURRENT_BINARY_DIR}/pypokelib_swig.py
)

INSTALL(
    FILES ${py_files}
    DESTINATION ${POKELIB_PYTHON_DIR}/PyPokeLib
    COMPONENT "PyPKMNsim"
)
INSTALL(
    TARGETS ${SWIG_MODULE_pypokelib_swig_REAL_NAME}
    DESTINATION ${POKELIB_PYTHON_DIR}/PyPokeLib
    COMPONENT "PyPKMNsim"
)
