#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
FILE(GLOB pkmnsim_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")

ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(types)

IF(NOT PKMNSIM_USED_AS_SUBMODULE)
    FOREACH(header ${pkmnsim_headers})
        INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/pkmnsim COMPONENT Headers)
    ENDFOREACH(header)
ENDIF(NOT PKMNSIM_USED_AS_SUBMODULE)
