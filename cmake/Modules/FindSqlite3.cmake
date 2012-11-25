# Find SQLite3
# ~~~~~~~~~~~~
# Copyright (c) 2007, Martin Dobias <wonder.sk at gmail.com>
# Copyright (c) 2012, Nicholas Corgan <n.corgan at gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# CMake module to search for SQLite3 library and executable
#
# If it's found it sets SQLITE3_FOUND to TRUE
# and following variables are set:
#    SQLITE3_INCLUDE_DIR
#    SQLITE3_LIBRARY
#    SQLITE3_EXECUTABLE


# FIND_PATH and FIND_LIBRARY normally search standard locations
# before the specified paths. To search non-standard paths first,
# FIND_* is invoked first with specified paths and NO_DEFAULT_PATH
# and then again with no specified paths to search the default
# locations. When an earlier FIND_* succeeds, subsequent FIND_*s
# searching for the same item do nothing. 

# try to use framework on mac
# want clean framework path, not unix compatibility path
IF (APPLE)
  IF (CMAKE_FIND_FRAMEWORK MATCHES "FIRST"
      OR CMAKE_FRAMEWORK_PATH MATCHES "ONLY"
      OR NOT CMAKE_FIND_FRAMEWORK)
    SET (CMAKE_FIND_FRAMEWORK_save ${CMAKE_FIND_FRAMEWORK} CACHE STRING "" FORCE)
    SET (CMAKE_FIND_FRAMEWORK "ONLY" CACHE STRING "" FORCE)
    #FIND_PATH(SQLITE3_INCLUDE_DIR SQLite3/sqlite3.h)
    FIND_LIBRARY(SQLITE3_LIBRARY SQLite3)
    IF (SQLITE3_LIBRARY)
      # FIND_PATH doesn't add "Headers" for a framework
      SET (SQLITE3_INCLUDE_DIR ${SQLITE3_LIBRARY}/Headers CACHE PATH "Path to a file.")
    ENDIF (SQLITE3_LIBRARY)
    SET (CMAKE_FIND_FRAMEWORK ${CMAKE_FIND_FRAMEWORK_save} CACHE STRING "" FORCE)
  ENDIF ()
ENDIF (APPLE)

FIND_PATH(SQLITE3_INCLUDE_DIR sqlite3.h
  "$ENV{LIB_DIR}/include"
  "$ENV{LIB_DIR}/include/sqlite"
  #mingw
  c:/msys/local/include
  NO_DEFAULT_PATH
  )
FIND_PATH(SQLITE3_INCLUDE_DIR sqlite3.h)

FIND_LIBRARY(SQLITE3_LIBRARY NAMES sqlite3 sqlite3_i PATHS
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  NO_DEFAULT_PATH
  )
FIND_LIBRARY(SQLITE3_LIBRARY NAMES sqlite3)

FIND_PROGRAM(SQLITE3_EXECUTABLE NAMES sqlite3)

IF (SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY AND SQLITE3_EXECUTABLE)
   SET(SQLITE3_FOUND TRUE)
ENDIF (SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY AND SQLITE3_EXECUTABLE)

IF (SQLITE3_FOUND)

   IF (NOT SQLITE3_FIND_QUIETLY)
      MESSAGE(STATUS "Found SQLite3:")
      MESSAGE(STATUS " * Library: ${SQLITE3_LIBRARY}")
      MESSAGE(STATUS " * Executable: ${SQLITE3_EXECUTABLE}")
   ENDIF (NOT SQLITE3_FIND_QUIETLY)

ELSE (SQLITE3_FOUND)

  MESSAGE(FATAL_ERROR "Could not find SQLite3")

ENDIF (SQLITE3_FOUND)
