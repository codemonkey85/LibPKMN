#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${CSHARP_USE_FILE})
INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")

SET(LIBPKMN_CSHARP_INCLUDE_DIRS
    ${LIBPKMN_SOURCE_DIR}/include
    ${LIBPKMN_SWIG_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)

SET(LIBPKMN_CSHARP_LIBRARIES
    pkmn
)

INCLUDE_DIRECTORIES(${LIBPKMN_CSHARP_INCLUDE_DIRS})
SET(CSHARP_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources CACHE INTERNAL "")
SET(CMAKE_SWIG_OUTDIR ${CSHARP_SOURCE_DIRECTORY})
FILE(MAKE_DIRECTORY ${CSHARP_SOURCE_DIRECTORY})

#Generate enums
ADD_CUSTOM_COMMAND(
    OUTPUT ${CSHARP_SOURCE_DIRECTORY}/Forms.cs
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=cs --database-path=${LIBPKMN_BINARY_DIR}/share/libpkmn/libpkmn.db --output-dir=${CSHARP_SOURCE_DIRECTORY}
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating C# enums"
)
ADD_CUSTOM_TARGET(
    cs_enums ALL
    DEPENDS ${CSHARP_SOURCE_DIRECTORY}/Forms.cs
    DEPENDS libpkmn.db
)

INCLUDE(UseSWIG)
MACRO(CSHARP_BUILD_MODULE swig_module_name dll_name cs_module_name)
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
    SWIG_LINK_LIBRARIES(${swig_module_name} ${LIBPKMN_CSHARP_LIBRARIES})

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
ENDMACRO(CSHARP_BUILD_MODULE)

####################################################################
# Build SWIG modules for different parts of LibPKMN
####################################################################
CSHARP_BUILD_MODULE(libpkmn_cs_swig libpkmn_cs_swig LibPKMN)
CSHARP_BUILD_MODULE(calculations_cs_swig calculations_cs_swig Calculations)
CSHARP_BUILD_MODULE(database_cs_swig database_cs_swig Database)
CSHARP_BUILD_MODULE(io_cs_swig io_cs_swig IO)

SET(cs_sources
    AssemblyInfo.cs
)
FOREACH(src ${cs_sources})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/${src}
    COPYONLY)
ENDFOREACH(src ${cs_sources})

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libpkmn-cs.dll
    COMMAND ${CSHARP_COMPILER} ARGS "/t:library" "/platform:x86" "/out:${CMAKE_CURRENT_BINARY_DIR}/libpkmn-cs.dll" "*.cs"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources
    DEPENDS ${SWIG_MODULE_libpkmn_cs_swig_REAL_NAME}
    COMMENT "Building libpkmn-cs.dll"
)

ADD_CUSTOM_TARGET(
    libpkmn-cs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libpkmn-cs.dll
)


INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libpkmn-cs.dll
    DESTINATION ${RUNTIME_DIR}
    COMPONENT "LibPKMN_CS"
)
