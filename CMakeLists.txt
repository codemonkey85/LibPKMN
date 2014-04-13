#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    SET(LIBPKMN_USED_AS_SUBMODULE TRUE CACHE BOOL "Part of larger project")
ELSE()
    SET(LIBPKMN_USED_AS_SUBMODULE FALSE CACHE BOOL "Part of larger project")
    PROJECT(LibPKMN)
    SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
    
    SET(LibPKMN_VERSION_MAJOR 0)
    SET(LibPKMN_VERSION_MINOR 0)
    SET(LibPKMN_VERSION_PATCH 1)
    
    ########################################################################
    # Create uninstall target
    ########################################################################
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY)

    ADD_CUSTOM_TARGET(uninstall
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
    
    ########################################################################
    # Set build type to Release by default to enable optimization flags
    ########################################################################
    IF(NOT CMAKE_BUILD_TYPE)
        SET(CMAKE_BUILD_TYPE "Release")
    ENDIF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "")
    
    ####################################################################
    # If this is set for a Windows build, all DLL dependencies will be
    # installed as well
    ####################################################################
    SET(WIN7_PKG ${WIN7_PKG} CACHE BOOL "Building a Windows package")
ENDIF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIBPKMN_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "LibPKMN source directory")
SET(LIBPKMN_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "LibPKMN binary directory")
INCLUDE(LibPKMNComponent)

########################################################################
# Automatic git submodule update
########################################################################
FIND_PACKAGE(Git)
MESSAGE(STATUS "")
MESSAGE(STATUS "Updating Git submodules. This may take a while...")
EXECUTE_PROCESS(
    COMMAND ${GIT_EXECUTABLE} submodule update --init
    OUTPUT_QUIET
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}
)

########################################################################
# Getting compilation info
########################################################################
IF(NOT LIBPKMN_USED_AS_SUBMODULE)
    INCLUDE(${LIBPKMN_SOURCE_DIR}/cmake/Modules/LibPKMNConfigInfo.cmake)
    MESSAGE(STATUS "")
ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)

INCLUDE(LibPKMNCompilerCheck)

####################################################################
# Use boost::shared_ptr if user specifies
####################################################################
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR USE_BOOST_SHARED_PTR)
    MESSAGE(STATUS "Using boost::shared_ptr for pkmn::shared_ptr.")
    MESSAGE(STATUS "")
    ADD_DEFINITIONS(-DLIBPKMN_USE_BOOST_SHARED_PTR)
ENDIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR USE_BOOST_SHARED_PTR)

####################################################################
# Find dependencies and set global include and link options
####################################################################
IF(NOT MSVC)
    SET(BOOST_COMPONENTS
        filesystem
        locale
        program_options
        system
        unit_test_framework
    )
ENDIF(NOT MSVC)
FIND_PACKAGE(Boost 1.48.0 COMPONENTS
    ${BOOST_COMPONENTS}
REQUIRED)
IF(NOT Boost_FOUND)
MESSAGE(FATAL_ERROR "Fatal error: Boost (version >= 1.48.0) required.\n")
ENDIF(NOT Boost_FOUND)
FIND_PROGRAM(SQLITE3_EXECUTABLE NAMES sqlite3)
FIND_PACKAGE(PythonInterp REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(UseQt4)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

####################################################################
# Set install subdirectories
####################################################################
IF(NOT DEFINED LIB_SUFFIX AND REDHAT AND CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
    SET(LIB_SUFFIX 64)
ENDIF()
SET(LIB_SUFFIX ${LIB_SUFFIX} CACHE STRING "lib directory suffix")
SET(RUNTIME_DIR bin)
SET(LIBRARY_DIR lib${LIB_SUFFIX})
SET(INCLUDE_DIR include)
SET(PKG_DATA_DIR share/libpkmn)
SET(DOCS_DIR share/doc/libpkmn)

########################################################################
# Register LibLibPKMN components
########################################################################
LIBPKMN_REGISTER_COMPONENT("LibPKMN - Core Library" ENABLE_LIBPKMN ON "Boost_FOUND;PYTHONINTERP_FOUND;SQLITE3_EXECUTABLE;QT4_FOUND" OFF)
LIBPKMN_REGISTER_COMPONENT("Utilities" ENABLE_UTILITIES ON "ENABLE_LIBPKMN" OFF)
LIBPKMN_REGISTER_COMPONENT("Applications" ENABLE_APPS ON "ENABLE_LIBPKMN" OFF)
LIBPKMN_REGISTER_COMPONENT("Unit Tests" ENABLE_TESTS ON "ENABLE_LIBPKMN" OFF)

INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(rpokesav)
ADD_SUBDIRECTORY(pokehack)
ADD_SUBDIRECTORY(PokeLib-NC)
ADD_SUBDIRECTORY(PKMDS)

ADD_SUBDIRECTORY(share)
ADD_SUBDIRECTORY(include)


ADD_SUBDIRECTORY(lib)
IF(NOT LIBPKMN_USED_AS_SUBMODULE)
    ADD_SUBDIRECTORY(docs)
    ADD_SUBDIRECTORY(utils)
    ADD_SUBDIRECTORY(apps)
    IF(ENABLE_TESTS)
        ENABLE_TESTING()
        ADD_SUBDIRECTORY(tests)
    ENDIF(ENABLE_TESTS)
ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)

########################################################################
# Set package settings
########################################################################
INCLUDE(LibPKMNPackage)

########################################################################
# Final display
########################################################################
IF(LIBPKMN_USED_AS_SUBMODULE)
    MESSAGE(STATUS "Building LibPKMN as part of ${CMAKE_PROJECT_NAME}.")
ELSE()
    LIBPKMN_PRINT_COMPONENT_SUMMARY()
    MESSAGE(STATUS "Building LibPKMN.")
    MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS " - Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    IF(WIN32 AND WIN7_PKG)
        MESSAGE(STATUS " - Build configured for Windows installer.")
    ELSE()
        MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
    ENDIF(WIN32 AND WIN7_PKG)
    MESSAGE(STATUS "")
ENDIF(LIBPKMN_USED_AS_SUBMODULE)
