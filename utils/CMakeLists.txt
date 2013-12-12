#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(ENABLE_UTILITIES)
    ADD_EXECUTABLE(pkmnsim-about pkmnsim-about.cpp)
    INCLUDE_DIRECTORIES(${PKMNSIM_BINARY_DIR}/include)
    TARGET_LINK_LIBRARIES(pkmnsim-about pkmnsim)
    INSTALL(TARGETS pkmnsim-about RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)

    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmnsim-config-info.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-config-info.cpp
    @ONLY)
    ADD_EXECUTABLE(pkmnsim-config-info ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-config-info.cpp)
    INSTALL(TARGETS pkmnsim-config-info RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
ENDIF(ENABLE_UTILITIES)
