#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(PKMNSIM_PYTHON_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}
    ${PKMNSIM_SWIG_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)
INCLUDE_DIRECTORIES(${PKMNSIM_PYTHON_INCLUDE_DIRS})

SET(PKMNSIM_PYTHON_LIBRARIES
    pkmnsim
    ${PYTHON_LIBRARIES}
)

####################################################################
# Namespace pkmnsim::database
####################################################################
INCLUDE(UseSWIG)
SET(CMAKE_SWIG_FLAGS -module database_swig)
FOREACH(dir ${PKMNSIM_PYTHON_INCLUDE_DIRS})
    LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
ENDFOREACH(dir ${PKMNSIM_PYTHON_INCLUDE_DIRS})
SET_SOURCE_FILES_PROPERTIES(database_swig.i PROPERTIES CPLUSPLUS ON)

SWIG_ADD_MODULE(database_swig python database_swig.i)
IF(CMAKE_COMPILER_IS_GNUCXX)
    IF(UNIX)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_database_swig_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
    ELSE() #Cygwin
        SET_TARGET_PROPERTIES(${SWIG_MODULE_database_swig_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ENDIF(UNIX)
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET_TARGET_PROPERTIES(${SWIG_MODULE_database_swig_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
SWIG_LINK_LIBRARIES(database_swig ${PKMNSIM_PYTHON_LIBRARIES})

FILE(GLOB py_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.py")
LIST(APPEND py_files ${CMAKE_CURRENT_BINARY_DIR}/database_swig.py)

INSTALL(
    FILES ${py_files}
    DESTINATION ${PKMNSIM_PYTHON_DIR}/PyPKMNsim/database
    COMPONENT "PyPKMNsim"
)
INSTALL(
    TARGETS ${SWIG_MODULE_database_swig_REAL_NAME}
    DESTINATION ${PKMNSIM_PYTHON_DIR}/PyPKMNsim/database
    COMPONENT "PyPKMNsim"
)
