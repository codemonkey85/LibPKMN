#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${PKMNSIM_SOURCE_DIR}/lib/swig/pkmnsim" PKMNSIM_SWIG_SOURCE_DIR)
SET(PKMNSIM_SWIG_SOURCE_DIR ${PKMNSIM_SWIG_SOURCE_DIR} CACHE FILEPATH "PKMNsim SWIG source directory")

####################################################################
# Check for PyQt4 and register component if found
####################################################################
PYTHON_CHECK_MODULE("PyQt4" PyQt4 True PYQT4_FOUND)
REGISTER_COMPONENT("PyPKMNsim Library - PyQt4 Subclasses" ENABLE_PYPKMNSIM_PYQT4 ON
                   "ENABLE_PYPKMNSIM_LIB;PYQT4_FOUND" OFF)

####################################################################
# Python install directory is system-specific, so use this
# script to determine this system's install directory
####################################################################
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "
from distutils import sysconfig
print sysconfig.get_python_lib(plat_specific=True, prefix='')
" OUTPUT_VARIABLE PKMNSIM_PYTHON_DIR OUTPUT_STRIP_TRAILING_WHITESPACE
)
file(TO_CMAKE_PATH ${PKMNSIM_PYTHON_DIR} PKMNSIM_PYTHON_DIR)
SET(PKMNSIM_PYTHON_DIR ${PKMNSIM_PYTHON_DIR} CACHE FILEPATH "Python install directory")

INCLUDE(${SWIG_USE_FILE})

SET(PKMNSIM_PYTHON_INCLUDE_DIRS
    ${PKMNSIM_SOURCE_DIR}/include
    ${PKMNSIM_SWIG_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)
INCLUDE_DIRECTORIES(${PKMNSIM_PYTHON_INCLUDE_DIRS})

SET(CMAKE_SWIG_FLAGS -module pypkmnsim_swig)
FOREACH(dir ${PKMNSIM_PYTHON_INCLUDE_DIRS})
    LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
ENDFOREACH(dir ${PKMNSIM_PYTHON_INCLUDE_DIRS})

SET(PKMNSIM_PYTHON_LIBRARIES
    pkmnsim
    ${PYTHON_LIBRARIES}
)

SET_SOURCE_FILES_PROPERTIES(pypkmnsim_swig.i PROPERTIES CPLUSPLUS ON)

####################################################################
# Create SWIG Python wrapper of LibPKMNsim
####################################################################
INCLUDE(UseSWIG)
SWIG_ADD_MODULE(pypkmnsim_swig python pypkmnsim_swig.i)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_TARGET_PROPERTIES(${SWIG_MODULE_pypkmnsim_swig_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET_TARGET_PROPERTIES(${SWIG_MODULE_pypkmnsim_swig_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
SWIG_LINK_LIBRARIES(pypkmnsim_swig ${PKMNSIM_PYTHON_LIBRARIES})

SET(PYPKMNSIM_REAL_SWIG_NAME ${SWIG_MODULE_pypkmnsim_swig_REAL_NAME} CACHE STRING "Real SWIG module name")

FILE(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/__init__.py")
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
    ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
COPYONLY)

LIST(APPEND py_files
    ${CMAKE_CURRENT_SOURCE_DIR}/Forms.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Games.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Genders.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Move_Classes.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Statuses.py
    ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
    ${CMAKE_CURRENT_BINARY_DIR}/pypkmnsim_swig.py
)

IF(ENABLE_PYPKMNSIM_PYQT4)
    FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/__init__.py "import Qt4")
    LIST(APPEND py_files ${CMAKE_CURRENT_SOURCE_DIR}/Qt4.py)
ENDIF(ENABLE_PYPKMNSIM_PYQT4)

INSTALL(
    FILES ${py_files}
    DESTINATION ${PKMNSIM_PYTHON_DIR}/PyPKMNsim
    COMPONENT "PyPKMNsim"
)
INSTALL(
    TARGETS ${SWIG_MODULE_pypkmnsim_swig_REAL_NAME}
    DESTINATION ${PKMNSIM_PYTHON_DIR}/PyPKMNsim
    COMPONENT "PyPKMNsim"
)
