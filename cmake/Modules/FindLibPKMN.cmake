#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_LIBPKMN pkmn)

FIND_PATH(
    LIBPKMN_INCLUDE_DIRS
    NAMES pkmn/config.hpp
    HINTS $ENV{LIBPKMN_DIR}/include
        ${PC_LIBPKMN_INCLUDEDIR}
    PATHS /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    LIBPKMN_LIBRARIES
    NAMES pkmn
    HINTS $ENV{LIBPKMN_DIR}/lib
        ${PC_LIBPKMN_LIBDIR}
    PATHS /usr/local/lib
          /usr/lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBPKMN DEFAULT_MSG LIBPKMN_LIBRARIES LIBPKMN_INCLUDE_DIRS)
MARK_AS_ADVANCED(LIBPKMN_LIBRARIES LIBPKMN_INCLUDE_DIRS)
