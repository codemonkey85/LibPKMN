#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(ENABLE_CMD_CPP_APPS)
    ADD_EXECUTABLE(team_analysis team_analysis.cpp)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_TARGET_PROPERTIES(team_analysis PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(team_analysis PROPERTIES COMPILE_FLAGS "-std=c++11")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
    TARGET_LINK_LIBRARIES(team_analysis pkmnsim)
    INSTALL(TARGETS team_analysis RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
ENDIF(ENABLE_CMD_CPP_APPS)
