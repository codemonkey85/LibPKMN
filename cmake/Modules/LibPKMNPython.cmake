#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying FILE LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# Check for the existence of a python module:
# - desc a string description of the check
# - mod the name of the module to import
# - cmd an additional command to run
# - have the result variable to SET
########################################################################
MACRO(PYTHON_CHECK_MODULE desc mod cmd have)
    MESSAGE(STATUS "")
    EXECUTE_PROCESS(
        COMMAND ${PYTHON_EXECUTABLE} -c "
#########################################
try:
    import ${mod}
    assert ${cmd}
except ImportError, AssertionError: exit(-1)
except: pass
#########################################"
        RESULT_VARIABLE ${have}
    )
    IF(${have} EQUAL 0)
        MESSAGE(STATUS "Checking for Python module ${desc} - found")
        SET(${have} TRUE)
    ELSE(${have} EQUAL 0)
        MESSAGE(STATUS "Checking for Python module ${desc} - not found")
        SET(${have} FALSE)
    ENDIF(${have} EQUAL 0)
ENDMACRO(PYTHON_CHECK_MODULE)

########################################################################
# Build and install Python SWIG modules
# Assumes LIBPKMN_SWIG_SOURCE_DIR is set
########################################################################
MACRO(PYTHON_BUILD_SWIG_MODULE module_name install_dir)
    EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "
from distutils import sysconfig
print sysconfig.get_python_lib(plat_specific=True, prefix='')
    " OUTPUT_VARIABLE LIBPKMN_PYTHON_DIR OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    FILE(TO_CMAKE_PATH ${LIBPKMN_PYTHON_DIR} LIBPKMN_PYTHON_DIR)
    SET(LIBPKMN_PYTHON_DIR ${LIBPKMN_PYTHON_DIR} CACHE FILEPATH "Python install directory")

    INCLUDE(${SWIG_USE_FILE})

    SET(LIBPKMN_PYTHON_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}
        ${LIBPKMN_SWIG_SOURCE_DIR}
        ${LIBPKMN_SWIG_SOURCE_DIR}/python
        ${Boost_INCLUDE_DIRS}
        ${PYTHON_INCLUDE_DIRS}
    )
    INCLUDE_DIRECTORIES(${LIBPKMN_PYTHON_INCLUDE_DIRS})

    SET(LIBPKMN_PYTHON_LIBRARIES
        pkmn
        ${PYTHON_LIBRARIES}
    )

    #TODO: better solution
    IF(${module_name} STREQUAL "qt4_swig")
        LIST(APPEND LIBPKMN_PYTHON_LIBRARIES
            qt4proxy
        )
    ENDIF(${module_name} STREQUAL "qt4_swig")

    INCLUDE(UseSWIG)
    SET(CMAKE_SWIG_FLAGS -module ${module_name})
    FOREACH(dir ${LIBPKMN_PYTHON_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${LIBPKMN_PYTHON_INCLUDE_DIRS})
    SET_SOURCE_FILES_PROPERTIES(${module_name}.i PROPERTIES CPLUSPLUS ON)

    SWIG_ADD_MODULE(${module_name} python ${module_name}.i)
    ADD_DEPENDENCIES(${SWIG_MODULE_${module_name}_REAL_NAME} python_enums)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        IF(UNIX)
            SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
        ELSE() #Cygwin
            SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
        ENDIF(UNIX)
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
    SWIG_LINK_LIBRARIES(${module_name} ${LIBPKMN_PYTHON_LIBRARIES})
    
    # Copy __init__.py to binary directory for unit tests
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
        ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
    @ONLY)

    SET(py_files
        ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.py
    )

    INSTALL(
        FILES ${py_files}
        DESTINATION ${LIBPKMN_PYTHON_DIR}/${install_dir}
        COMPONENT "LibPKMN_Python"
    )
    INSTALL(
        TARGETS ${SWIG_MODULE_${module_name}_REAL_NAME}
        DESTINATION ${LIBPKMN_PYTHON_DIR}/${install_dir}
        COMPONENT "LibPKMN_Python"
    )

ENDMACRO(PYTHON_BUILD_SWIG_MODULE)
