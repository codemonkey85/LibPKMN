#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
FILE(GLOB trainer_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.txt")
STRING(REGEX REPLACE "CMake[^;]+;?" "" trainer_files "${trainer_files}")

FOREACH(team ${trainer_files})
    INSTALL(FILES ${team} DESTINATION ${PKG_DATA_DIR}/sample_teams/black2_white2 COMPONENT "trainer file")
ENDFOREACH(team)
