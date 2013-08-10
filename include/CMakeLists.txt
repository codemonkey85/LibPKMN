#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
ADD_SUBDIRECTORY(pkmnsim)

#Install submodule headers despite their CMake configurations to the contrary
FILE(TO_CMAKE_PATH "${PKMNSIM_SOURCE_DIR}/pokehack/include/pokehack" POKEHACK_INCLUDE_DIR)
FILE(TO_CMAKE_PATH "${PKMNSIM_SOURCE_DIR}/PokeLib/include/pokelib" POKELIB_INCLUDE_DIR)
FILE(TO_CMAKE_PATH "${PKMNSIM_SOURCE_DIR}/PKMDS-G5/include/pkmds" PKMDS_INCLUDE_DIR)

FILE(GLOB pokehack_headers "${POKEHACK_INCLUDE_DIR}/*.h")
FILE(GLOB pokelib_headers "${POKELIB_INCLUDE_DIR}/*.h")
FILE(GLOB pkmds_headers "${PKMDS_INCLUDE_DIR}/*.h")

IF(NOT PKMNSIM_USED_AS_SUBMODULE)
    INSTALL(FILES ${pokehack_headers} DESTINATION ${INCLUDE_DIR}/pokehack COMPONENT Headers)
    INSTALL(FILES ${pokelib_headers} DESTINATION ${INCLUDE_DIR}/pokelib COMPONENT Headers)
    INSTALL(FILES ${pkmds_headers} DESTINATION ${INCLUDE_DIR}/pkmds COMPONENT Headers)
ENDIF(NOT PKMNSIM_USED_AS_SUBMODULE)
