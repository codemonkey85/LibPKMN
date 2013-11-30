#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(${PKMNSIM_BINARY_DIR}/include)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(MOCSrcs GetTypeStatsWindow.hpp OptionsGroupBox.hpp CalculationOutput.hpp)

LIST(APPEND gui_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/get_type_stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CalculationOutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetTypeStatsWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OptionsGroupBox.cpp
    ${MOCSrcs}
)

ADD_EXECUTABLE(get_type_stats WIN32 ${gui_sources} type_stats_common.cpp)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_TARGET_PROPERTIES(get_type_stats PROPERTIES COMPILE_FLAGS "-std=c++0x")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET_TARGET_PROPERTIES(get_type_stats PROPERTIES COMPILE_FLAGS "-std=c++11")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
TARGET_LINK_LIBRARIES(get_type_stats pkmnsim ${QT_QTMAIN_LIBRARY})
IF(MSVC)
    ####################################################################
    # This setting is necessary so opening the application from the
    # Start Menu won't bring up a command line
    ####################################################################
    SET_TARGET_PROPERTIES(get_type_stats PROPERTIES LINK_FLAGS_RELEASE "/subsystem:windows")
    
    ####################################################################
    # Use get_type_stats.exe to determine which DLL's to install
    # if WIN7_PKG=ON is set when running CMake
    ####################################################################
    IF(WIN7_PKG)
        INCLUDE(GetPrerequisites)

        SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/get_type_stats.exe")
        INSTALL(CODE
            "INCLUDE(BundleUtilities)
            fixup_bundle(\"${APPS}\" \"\" \"\")"
        COMPONENT Runtime)
    ENDIF(WIN7_PKG)
ENDIF(MSVC)

INSTALL(TARGETS get_type_stats RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)