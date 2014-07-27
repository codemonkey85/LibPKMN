#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# Build and install C# SWIG modules
# Assumes LIBPKMN_SWIG_SOURCE_DIR is set
########################################################################
MACRO(CSHARP_BUILD_SWIG_MODULE swig_module_name dll_name cs_module_name)
    FOREACH(CMAKE_CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        STRING(TOUPPER ${CMAKE_CONFIGURATION_TYPE} CMAKE_CONFIGURATION_TYPE)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_TARGET_NAME}
            PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${CMAKE_CONFIGURATION_TYPE} "${CSHARP_BINARY_DIRECTORY}")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_TARGET_NAME}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${CMAKE_CONFIGURATION_TYPE} "${CSHARP_BINARY_DIRECTORY}")
    ENDFOREACH()

    SET_SOURCE_FILES_PROPERTIES(${swig_module_name}.i PROPERTIES CPLUSPLUS ON)
    IF(UNIX)
        SET(CMAKE_SWIG_FLAGS -module ${cs_module_name} -namespace \"LibPKMN\" -dllimport \"lib${dll_name}\" ${CMAKE_SWIG_GLOBAL_FLAGS} ${CMAKE_GLOBAL_FLAGS})
    ELSEIF(WIN32)
        SET(CMAKE_SWIG_FLAGS -module ${cs_module_name} -namespace \"LibPKMN\" -dllimport \"${dll_name}\" ${CMAKE_SWIG_GLOBAL_FLAGS} ${CMAKE_GLOBAL_FLAGS})
    ENDIF(UNIX)

    SWIG_ADD_MODULE(${swig_module_name} csharp ${swig_module_name}.i)
    ADD_DEPENDENCIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} cs_enums)
    IF(UNIX)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES PREFIX "lib")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES SUFFIX ".so")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ELSEIF(WIN32)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES PREFIX "")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES SUFFIX ".dll")
    ENDIF(UNIX)
    SWIG_LINK_LIBRARIES(${swig_module_name} pkmn)

    IF(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_module_name}_REAL_NAME}
            DESTINATION ${RUNTIME_DIR}
            COMPONENT "LibPKMN_CS"
        )
    ELSE(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_module_name}_REAL_NAME}
            DESTINATION ${LIBRARY_DIR}
            COMPONENT "LibPKMN_CS"
        )
    ENDIF(WIN32)
ENDMACRO(CSHARP_BUILD_SWIG_MODULE)
