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
        ${PKMNSIM_BINARY_DIR}/include/pkmnsim/qt4/${header}
    )
    ####################################################################
    # These headers are needed here for the Qt4 subclass build, but
    # for runtime, they need to be in the normal header directory
    ####################################################################
    IF(NOT PKMNSIM_USED_AS_SUBMODULE)
        INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/pkmnsim/qt4 COMPONENT Headers)
    ENDIF(NOT PKMNSIM_USED_AS_SUBMODULE)
ENDFOREACH(header)
