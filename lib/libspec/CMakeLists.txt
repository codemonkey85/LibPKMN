#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(libspec_sources
    game_gba.c
    checksum.c
    stat.c
)

ADD_LIBRARY(spec STATIC ${libspec_sources})
IF(UNIX)
    SET_TARGET_PROPERTIES(spec PROPERTIES COMPILE_FLAGS "-std=c99 -fPIC")
ENDIF(UNIX)
