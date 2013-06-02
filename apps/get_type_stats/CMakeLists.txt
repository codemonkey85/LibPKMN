#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "Get Type Stats")
IF(Boost_FOUND AND SQLITE3_FOUND)
    MESSAGE(STATUS " * Building command line app.")

    ADD_EXECUTABLE(get_type_stats get_type_stats.cpp)
    TARGET_LINK_LIBRARIES(get_type_stats pkmnsim)
    INSTALL(TARGETS get_type_stats RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT apps)
ENDIF(Boost_FOUND AND SQLITE3_FOUND)
IF(Boost_FOUND AND SQLITE3_FOUND AND QT4_FOUND)
    MESSAGE(STATUS " * Building GUI app.")

    INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)
    INCLUDE(${QT_USE_FILE})

    QT4_WRAP_CPP(MOCSrcs GetTypeStatsWindow.hpp OptionsGroupBox.hpp)

    LIST(APPEND gui_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/get_type_stats_gui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GetTypeStatsWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OptionsGroupBox.cpp
        ${MOCSrcs}
    )

    ADD_EXECUTABLE(get_type_stats_gui ${gui_sources})
    TARGET_LINK_LIBRARIES(get_type_stats_gui pkmnsim)
    IF(MSVC)
        SET_TARGET_PROPERTIES(get_type_stats_gui PROPERTIES LINK_FLAG_RELEASE "/SUBSYSTEM:WINDOWS")
    ENDIF(MSVC)

    INSTALL(TARGETS get_type_stats_gui RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT apps)
ENDIF(Boost_FOUND AND SQLITE3_FOUND AND QT4_FOUND)
