#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
FILE(GLOB pkmn_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")

ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(types)

IF(ENABLE_LIBPKMN_QT)
    ADD_SUBDIRECTORY(qt)
ENDIF(ENABLE_LIBPKMN_QT)

#####################################################################
# Only install headers if LibPKMN is built standalone
#####################################################################
IF(NOT LIBPKMN_USED_AS_SUBMODULE)
    FOREACH(header ${pkmn_headers})
        INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/pkmn COMPONENT Headers)
    ENDFOREACH(header)
ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)
