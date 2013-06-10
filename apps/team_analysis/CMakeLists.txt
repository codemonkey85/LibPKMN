#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(ENABLE_CMD_CPP_APPS)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/team_analysis_common.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/team_analysis_common.hpp
    )
    ADD_EXECUTABLE(team_analysis team_analysis.cpp)
    TARGET_LINK_LIBRARIES(team_analysis pkmnsim)
    INSTALL(TARGETS team_analysis RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
ENDIF(ENABLE_CMD_CPP_APPS)
