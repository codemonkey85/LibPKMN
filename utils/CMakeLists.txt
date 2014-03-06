#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(ENABLE_UTILITIES)
    IF(ENABLE_LIBPKMN_QT)
        ADD_EXECUTABLE(libpkmn-about libpkmn-about.cpp)
        INCLUDE_DIRECTORIES(${LIBPKMN_BINARY_DIR}/include)
        TARGET_LINK_LIBRARIES(libpkmn-about pkmn)
        IF(MSVC)
            ####################################################################
            # This setting is necessary so opening the application from the
            # Start Menu won't bring up a command line
            ####################################################################
            SET_TARGET_PROPERTIES(libpkmn-about PROPERTIES LINK_FLAGS_RELEASE "/subsystem:windows")
        ENDIF(MSVC)
        INSTALL(TARGETS libpkmn-about RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
    ENDIF(ENABLE_LIBPKMN_QT)

    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/libpkmn-config-info.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/libpkmn-config-info.cpp
    @ONLY)
    ADD_EXECUTABLE(libpkmn-config-info ${CMAKE_CURRENT_BINARY_DIR}/libpkmn-config-info.cpp)
    INSTALL(TARGETS libpkmn-config-info RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
ENDIF(ENABLE_UTILITIES)
