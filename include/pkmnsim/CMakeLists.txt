#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
FILE(GLOB pkmnsim_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
LIST(APPEND pkmnsim_headers vla.ipp dict.ipp)

FOREACH(header ${pkmnsim_headers})
    INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/pkmnsim COMPONENT Headers)
ENDFOREACH(header)
