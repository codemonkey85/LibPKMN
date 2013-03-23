#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
FILE(GLOB pkmn_icons RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.png")

FOREACH(icon ${pkmn_icons})
    INSTALL(FILES ${icon} DESTINATION ${PKG_DATA_DIR}/images/icons COMPONENT "images")
ENDFOREACH(icon)
