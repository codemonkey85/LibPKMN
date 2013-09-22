#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

MESSAGE(FATAL_ERROR "Currently in development. Will not compile.")

IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    SET(PKMNSIM_USED_AS_SUBMODULE TRUE CACHE BOOL "Part of larger project")
ELSE()
    SET(PKMNSIM_USED_AS_SUBMODULE FALSE CACHE BOOL "Part of larger project")
    PROJECT(Pokemon-sim)
    SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
    
    SET(PKMNsim_VERSION_MAJOR 0)
    SET(PKMNsim_VERSION_MINOR 0)
    SET(PKMNsim_VERSION_PATCH 1)
    
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

SET(PKMNSIM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "PKMNsim source directory")
SET(PKMNSIM_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "PKMNsim binary directory")
INCLUDE(${PKMNSIM_SOURCE_DIR}/cmake/Modules/PKMNsimComponent.cmake)

########################################################################
# Testing automatic git submodule update
########################################################################
FIND_PACKAGE(Git)
MESSAGE(STATUS "")
MESSAGE(STATUS "Updating Git submodules. This may take a while...")
EXECUTE_PROCESS(
    COMMAND ${GIT_EXECUTABLE} submodule update --init
    OUTPUT_QUIET
    WORKING_DIRECTORY ${PKMNSIM_SOURCE_DIR}
)

########################################################################
# Getting compilation info
########################################################################
IF(NOT PKMNSIM_USED_AS_SUBMODULE)
    INCLUDE(${PKMNSIM_SOURCE_DIR}/cmake/Modules/PKMNsimConfigInfo.cmake)
    MESSAGE(STATUS "")
ENDIF(NOT PKMNSIM_USED_AS_SUBMODULE)

####################################################################
# Find dependencies and set global include and link options
####################################################################
FIND_PACKAGE(Boost 1.48.0 COMPONENTS
    filesystem
    program_options
    system
REQUIRED)
IF(NOT Boost_FOUND)
MESSAGE(FATAL_ERROR "Fatal error: Boost (version >= 1.48.0) required.\n")
ENDIF(NOT Boost_FOUND)
FIND_PROGRAM(SQLITE3_EXECUTABLE NAMES sqlite3)
IF(NOT PKMNSIM_USED_AS_SUBMODULE)
    FIND_PACKAGE(Qt4 REQUIRED)
    INCLUDE(UseQt4)
ENDIF(NOT PKMNSIM_USED_AS_SUBMODULE)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

IF(MSVC)
	ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK) #Let MSVC automatically link to Boost libs
ENDIF(MSVC)

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
SET(PKG_DATA_DIR share/pkmnsim)

########################################################################
# Register LibPKMNsim components
########################################################################
PKMNSIM_REGISTER_COMPONENT("Pokemon-sim Library" ENABLE_LIBRARY ON "Boost_FOUND;SQLITE3_EXECUTABLE;QT4_FOUND" OFF)
PKMNSIM_REGISTER_COMPONENT("Utilities" ENABLE_UTILITIES ON "ENABLE_LIBRARY" OFF)
PKMNSIM_REGISTER_COMPONENT("Applications" ENABLE_APPS ON "ENABLE_LIBRARY" OFF)

INCLUDE_DIRECTORIES(${PKMNSIM_SOURCE_DIR}/include)

########################################################################
# Don't rebuild save parsing libraries if built as part of PkmnLibEx
########################################################################
IF(NOT DEFINED PKMNLIBEX_SOURCE_DIR)
ADD_SUBDIRECTORY(pokehack)
ADD_SUBDIRECTORY(PokeLib)
ADD_SUBDIRECTORY(PKMDS-G5)
ENDIF(NOT DEFINED PKMNLIBEX_SOURCE_DIR)

ADD_SUBDIRECTORY(share)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(lib)
IF(NOT PKMNSIM_USED_AS_SUBMODULE)
    ADD_SUBDIRECTORY(utils)
    ADD_SUBDIRECTORY(apps)
ENDIF(NOT PKMNSIM_USED_AS_SUBMODULE)

########################################################################
# Set package settings
########################################################################
INCLUDE(${PKMNSIM_SOURCE_DIR}/cmake/Modules/PKMNsimPackage.cmake)

########################################################################
# Final display
########################################################################
IF(PKMNSIM_USED_AS_SUBMODULE)
    MESSAGE(STATUS "Building Pokemon-sim as part of ${CMAKE_PROJECT_NAME}.")
ELSE()
    PKMNSIM_PRINT_COMPONENT_SUMMARY()
    MESSAGE(STATUS "Building Pokemon-sim.")
    MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS " - Compiler: ${CMAKE_CXX_COMPILER_ID}")
    IF(WIN32 AND WIN7_PKG)
        MESSAGE(STATUS " - Build configured for Windows installer.")
    ELSE()
        MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
    ENDIF(WIN32 AND WIN7_PKG)
    MESSAGE(STATUS "")
ENDIF(PKMNSIM_USED_AS_SUBMODULE)
