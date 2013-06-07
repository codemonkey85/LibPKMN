#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(ENABLE_CMD_CPP_APPS)
    ADD_EXECUTABLE(get_type_stats get_type_stats.cpp type_stats_common.cpp)
    TARGET_LINK_LIBRARIES(get_type_stats pkmnsim)
    INSTALL(TARGETS get_type_stats RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT apps)
    
    #Use get_type_stats to determine all necessary DLL's for Windows to bring in
    #if necessary (pkmnsim.dll will cover all Qt DLL's)
    #NOTE: All necessary DLL's must be in the PATH
    IF(WIN7_PKG)
        INCLUDE(GetPrerequisites)

        SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/get_type_stats.exe")
        INSTALL(CODE
            "INCLUDE(BundleUtilities)
            fixup_bundle(\"${APPS}\" \"\" \"\")"
        COMPONENT Runtime)
    ENDIF(WIN7_PKG)    
ENDIF(ENABLE_CMD_CPP_APPS)

IF(ENABLE_QT4_CPP_APPS)
    INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)
    INCLUDE(${QT_USE_FILE})

    QT4_WRAP_CPP(MOCSrcs GetTypeStatsWindow.hpp OptionsGroupBox.hpp CalculationOutput.hpp)

    LIST(APPEND gui_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/get_type_stats_gui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CalculationOutput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GetTypeStatsWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OptionsGroupBox.cpp
        ${MOCSrcs}
    )

    ADD_EXECUTABLE(get_type_stats_gui ${gui_sources} type_stats_common.cpp)
    TARGET_LINK_LIBRARIES(get_type_stats_gui pkmnsim)
    IF(MSVC)
        SET_TARGET_PROPERTIES(get_type_stats_gui PROPERTIES LINK_FLAG_RELEASE "/SUBSYSTEM:WINDOWS")
    ENDIF(MSVC)

    INSTALL(TARGETS get_type_stats_gui RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT apps)
ENDIF(ENABLE_QT4_CPP_APPS)
