#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=cs --include-path=${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp --output-dir=${CSHARP_SOURCE_DIRECTORY}
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating C# enums"
)
ADD_CUSTOM_TARGET(
    cs_enums ALL
    DEPENDS ${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp
    DEPENDS ${CSHARP_SOURCE_DIRECTORY}/Forms.cs
)

####################################################################
# Build SWIG modules for different parts of LibPKMN
####################################################################
INCLUDE(UseSWIG)
INCLUDE(LibPKMNCSharp)
CSHARP_BUILD_SWIG_MODULE(libpkmn_cs_swig libpkmn_cs_swig LibPKMN)
CSHARP_BUILD_SWIG_MODULE(calculations_cs_swig calculations_cs_swig Calculations)
CSHARP_BUILD_SWIG_MODULE(database_cs_swig database_cs_swig Database)
CSHARP_BUILD_SWIG_MODULE(io_cs_swig io_cs_swig IO)

SET(cs_sources
    AssemblyInfo.cs
)
FOREACH(src ${cs_sources})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/${src}
    COPYONLY)
ENDFOREACH(src ${cs_sources})

SET(CSHARP_PLATFORM "x86")
SET(CSHARP_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
CSHARP_ADD_LIBRARY(
    libpkmn-cs "System.Windows.Forms.dll" "${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/*cs"
)
ADD_DEPENDENCIES(libpkmn-cs
    ${SWIG_MODULE_libpkmn_cs_swig_REAL_NAME}
    ${SWIG_MODULE_calculations_cs_swig_REAL_NAME}
    ${SWIG_MODULE_database_cs_swig_REAL_NAME}
    ${SWIG_MODULE_io_cs_swig_REAL_NAME}
)

INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libpkmn-cs.dll
    DESTINATION ${RUNTIME_DIR}
    COMPONENT "LibPKMN_CS"
)
