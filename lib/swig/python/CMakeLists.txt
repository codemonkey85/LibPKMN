#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/lib/swig" PKMNSIM_SWIG_SOURCE_DIR)
SET(PKMNSIM_SWIG_SOURCE_DIR ${PKMNSIM_SWIG_SOURCE_DIR} CACHE FILEPATH "PKMNsim SWIG source directory")

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
    ${CMAKE_SOURCE_DIR}
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
    pkmnsim-swig
    ${PYTHON_LIBRARIES}
)

SET_SOURCE_FILES_PROPERTIES(pypkmnsim_swig.i PROPERTIES CPLUSPLUS ON)

####################################################################
# Create SWIG Python wrapper of LibPKMNsim
####################################################################
INCLUDE(UseSWIG)
SWIG_ADD_MODULE(pypkmnsim_swig python pypkmnsim_swig.i)
IF(CMAKE_COMPILER_IS_GNUCXX)
    IF(LINUX)
        SET_TARGET_PROPERTIES(pypkmnsim_swig PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
    ELSE() #Cygwin
        SET_TARGET_PROPERTIES(pypkmnsim_swig PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ENDIF(LINUX)
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET_TARGET_PROPERTIES(pypkmnsim_swig PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
SWIG_LINK_LIBRARIES(pypkmnsim_swig ${PKMNSIM_PYTHON_LIBRARIES})

SET(PYPKMNSIM_REAL_SWIG_NAME ${SWIG_MODULE_pypkmnsim_swig_REAL_NAME} CACHE STRING "Real SWIG module name")

FILE(GLOB py_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.py")
LIST(APPEND py_files ${CMAKE_CURRENT_BINARY_DIR}/pypkmnsim_swig.py)

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
