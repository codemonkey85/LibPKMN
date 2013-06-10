#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
FILE(GLOB pkmnsim_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")

FOREACH(header ${pkmnsim_headers})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${header}
        ${CMAKE_BINARY_DIR}/include/pkmnsim/${header}
    )
    #These headers should only be installed if Qt4 subclasses are used
    INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/pkmnsim COMPONENT Headers)
ENDFOREACH(header)
