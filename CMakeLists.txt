#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)
PROJECT(PKMNsim)

SET(PKMNsim_VERSION_MAJOR 0)
SET(PKMNsim_VERSION_MINOR 0)
SET(PKMNsim_VERSION_PATCH 1)

####################################################################
# If this is set for a Windows build, all DLL dependencies will be
# installed as well
####################################################################
SET(WIN7_PKG ${WIN7_PKG} CACHE BOOL "Building a Windows package")

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
INCLUDE(PKMNsimComponent)

####################################################################
# Find dependencies and set global include and link options
####################################################################
FIND_PACKAGE(Boost COMPONENTS
    filesystem
    program_options
    system
REQUIRED)
FIND_PROGRAM(SQLITE3_EXECUTABLE NAMES sqlite3)
FIND_PACKAGE(Qt4)
IF(QT4_FOUND)
    INCLUDE(UseQt4)
ENDIF(QT4_FOUND)

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

########################################################################
# Register LibPKMNsim components
########################################################################
REGISTER_COMPONENT("PKMNsim Library - Core" ENABLE_LIBPKMNSIM ON "Boost_FOUND;SQLITE3_EXECUTABLE" OFF)
REGISTER_COMPONENT("PKMNsim Library - Qt4 Subclasses" ENABLE_LIBPKMNSIM_QT4 ON "ENABLE_LIBPKMNSIM;QT4_FOUND" OFF)

########################################################################
# Run CMake scripts in subdirectories
########################################################################
function(PKMNSIM_CHECK_SUBMODULE directory)
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${directory}/CMakeLists.txt)
        message(FATAL_ERROR "
Submodule ${directory} does not exist.
Did you remember to run the following commands?
    git submodule init
    git submodule update
")
    endif()
endfunction(PKMNSIM_CHECK_SUBMODULE)

PKMNSIM_CHECK_SUBMODULE(pokehack)
PKMNSIM_CHECK_SUBMODULE(PokeLib)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
ADD_SUBDIRECTORY(pokehack)
ADD_SUBDIRECTORY(PokeLib)
ADD_SUBDIRECTORY(share)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(apps)

########################################################################
# Set package settings
########################################################################
INCLUDE(PKMNsimPackage)

########################################################################
# Final display
########################################################################
PRINT_COMPONENT_SUMMARY()
MESSAGE(STATUS "Building PKMNsim.")
MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
IF(WIN32 AND WIN7_PKG)
    MESSAGE(STATUS " - Build configured for Windows installer.")
ELSE()
    MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
ENDIF(WIN32 AND WIN7_PKG)
MESSAGE(STATUS "")
