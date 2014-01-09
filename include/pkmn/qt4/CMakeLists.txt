#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
FILE(GLOB pkmn_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")

IF(NOT LIBPKMN_USED_AS_SUBMODULE)
    FOREACH(header ${pkmn_headers})
        INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/pkmn/qt4 COMPONENT Headers)
    ENDFOREACH(header)
ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)
