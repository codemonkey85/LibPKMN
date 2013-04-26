#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
cmake_minimum_required(VERSION 2.8.5)
project(PKMNsim)

set(pkmnsim_VERSION_MAJOR 0)
set(pkmnsim_VERSION_MINOR 1)

#Find dependencies
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
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
IF(NOT DEFINED PKG_LIB_DIR)
    SET(PKG_LIB_DIR ${PKG_DATA_DIR})
ENDIF()

#select the release build type by default to get optimization flags
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(share)
add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(apps)

MESSAGE(STATUS "Building PKMNsim.")
MESSAGE(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "")
