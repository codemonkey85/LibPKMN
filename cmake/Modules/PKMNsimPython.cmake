#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(PKMNsimMisc)

########################################################################
# Writes MIT License to beginning of generated file
########################################################################
macro(WRITE_PYTHON_LICENSE filename)
SET(PYTHON_LICENSE_TEXT "#
\# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
\#
\# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
\# or copy at http://opensource.org/licenses/MIT)
\#")
FILE(WRITE "${filename}" "${PYTHON_LICENSE_TEXT}")
endmacro(WRITE_PYTHON_LICENSE)

########################################################################
# Check for the existence of a python module:
# - desc a string description of the check
# - mod the name of the module to import
# - cmd an additional command to run
# - have the result variable to set
########################################################################
macro(PYTHON_CHECK_MODULE desc mod cmd have)
    message(STATUS "")
    execute_process(
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
    if(${have} EQUAL 0)
        message(STATUS "Checking for Python module ${desc} - found")
        set(${have} TRUE)
    else(${have} EQUAL 0)
        message(STATUS "Checking for Python module ${desc} - not found")
        set(${have} FALSE)
    endif(${have} EQUAL 0)
endmacro(PYTHON_CHECK_MODULE)

########################################################################
# Build and install Python SWIG modules
# Assumes PKMNSIM_SWIG_SOURCE_DIR is set
########################################################################
macro(PYTHON_BUILD_MODULE module_name install_dir)
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
        ${PKMNSIM_SWIG_SOURCE_DIR}/python
        ${Boost_INCLUDE_DIRS}
        ${PYTHON_INCLUDE_DIRS}
    )
    INCLUDE_DIRECTORIES(${PKMNSIM_PYTHON_INCLUDE_DIRS})

    SET(PKMNSIM_PYTHON_LIBRARIES
        pkmnsim
        ${PYTHON_LIBRARIES}
    )

    INCLUDE(UseSWIG)
    SET(CMAKE_SWIG_FLAGS -module ${module_name})
    FOREACH(dir ${PKMNSIM_PYTHON_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${PKMNSIM_PYTHON_INCLUDE_DIRS})
    SET_SOURCE_FILES_PROPERTIES(${module_name}.i PROPERTIES CPLUSPLUS ON)

    SWIG_ADD_MODULE(${module_name} python ${module_name}.i)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        IF(UNIX)
            SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
        ELSE() #Cygwin
            SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
        ENDIF(UNIX)
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
    SWIG_LINK_LIBRARIES(${module_name} ${PKMNSIM_PYTHON_LIBRARIES})

    FILE(GLOB py_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.py")
    LIST(APPEND py_files ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.py)

    INSTALL(
        FILES ${py_files}
        DESTINATION ${PKMNSIM_PYTHON_DIR}/${install_dir}
        COMPONENT "PyPKMNsim"
    )
    INSTALL(
        TARGETS ${SWIG_MODULE_${module_name}_REAL_NAME}
        DESTINATION ${PKMNSIM_PYTHON_DIR}/${install_dir}
        COMPONENT "PyPKMNsim"
    )

endmacro(PYTHON_BUILD_MODULE)

########################################################################
# Generates Python classes to correspond to C++ enums
########################################################################
macro(GENERATE_PYTHON_ENUM name)
    SET(ENUM_LOCATION ${PKMNSIM_BINARY_DIR}/lib/swig/python/${name}.py)
    FILE(TO_CMAKE_PATH ENUM_LOCATION ${ENUM_LOCATION})

    SET(LINE_NUMBER "")
    SET(ENUMS_FILE "${PKMNSIM_SOURCE_DIR}/include/pkmnsim/enums.hpp")
    #FILE(TO_NATIVE_PATH) doesn't replace / with \
    STRING(REPLACE "/" "\\\\" ENUMS_FILE ${ENUMS_FILE})
    GET_LINE_NUMBER(${name} ${ENUMS_FILE} 0)
    
    FILE(REMOVE ${ENUM_LOCATION})
    WRITE_PYTHON_LICENSE(${ENUM_LOCATION})
endmacro(GENERATE_PYTHON_ENUM)