#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
cmake_minimum_required(VERSION 2.8.5)
project(PKMNsim)

set(PKMNsim_VERSION_MAJOR 0)
set(PKMNsim_VERSION_MINOR 0)
set(PKMNsim_VERSION_PATCH 1)

SET(WIN7_PKG ${WIN7_PKG} CACHE BOOL "Building a Windows package")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
INCLUDE(PKMNsimComponent)

#Find dependencies
FIND_PACKAGE(Boost COMPONENTS
    filesystem
    program_options
    system
REQUIRED)
FIND_PACKAGE(Sqlite3 REQUIRED)
FIND_PACKAGE(Qt4)
IF(QT4_FOUND)
    INCLUDE(UseQt4)
ENDIF(QT4_FOUND)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} ${SQLITE3_LIBRARY_DIRS})

IF(MSVC)
	ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK) #Set up Boost dynamic linking
ENDIF(MSVC)

IF(NOT DEFINED LIB_SUFFIX AND REDHAT AND CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
    SET(LIB_SUFFIX 64)
ENDIF()
SET(LIB_SUFFIX ${LIB_SUFFIX} CACHE STRING "lib directory suffix")
SET(RUNTIME_DIR bin)
SET(LIBRARY_DIR lib${LIB_SUFFIX})
SET(INCLUDE_DIR include)
SET(PKG_DATA_DIR share/pkmnsim)

########################################################################
# Create Uninstall Target
########################################################################
CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)

ADD_CUSTOM_TARGET(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

#select the release build type by default to get optimization flags
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "")

#Register components
REGISTER_COMPONENT("PKMNsim Library - Core" ENABLE_LIBPKMNSIM ON "Boost_FOUND;SQLITE3_FOUND" OFF)
REGISTER_COMPONENT("PKMNsim Library - Qt4 Subclasses" ENABLE_LIBPKMNSIM_QT4 ON "ENABLE_LIBPKMNSIM;QT4_FOUND" OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(pokehack)
add_subdirectory(share)
add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(apps)

INCLUDE(PKMNsimPackage)

PRINT_COMPONENT_SUMMARY()
MESSAGE(STATUS "Building PKMNsim.")
MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
IF(WIN32 AND WIN7_PKG)
    MESSAGE(STATUS " - Build configured for Windows installer.")
ELSE()
    MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
ENDIF(WIN32 AND WIN7_PKG)
MESSAGE(STATUS "")
