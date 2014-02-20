#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PACKAGE(Doxygen)

LIBPKMN_REGISTER_COMPONENT("Doxygen - API Documentation" ENABLE_DOXYGEN ON "DOXYGEN_FOUND;ENABLE_LIBPKMN" OFF)

IF(ENABLE_DOXYGEN)
    #Generate the Doxygen configuration file
    SET(DOXYGEN_CPP_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/cpp")
    MAKE_DIRECTORY(${DOXYGEN_CPP_OUTPUT_DIR})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
    @ONLY)
    
    FILE(GLOB_RECURSE header_files ${LIBPKMN_SOURCE_DIR}/include/*.hpp)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${DOXYGEN_CPP_OUTPUT_DIR}/html/index.html DEPENDS ${header_files}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
        COMMENT "Generating C++ API documentation with Doxygen"
    )
    ADD_CUSTOM_TARGET(doxygen_cpp_docs ALL DEPENDS ${DOXYGEN_CPP_OUTPUT_DIR}/html/index.html)
    INSTALL(DIRECTORY ${DOXYGEN_CPP_OUTPUT_DIR} DESTINATION ${DOCS_DIR}/cpp COMPONENT doxygen)

    IF(ENABLE_CSHARP)
        SET(DOXYGEN_CS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/cs")
        MAKE_DIRECTORY(${DOXYGEN_CS_OUTPUT_DIR})
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_cs.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cs
        @ONLY)

        FILE(GLOB_RECURSE cs_source_files ${LIBPKMN_BINARY_DIR}/lib/swig/cs/CSharpSources/*.cs)
        ADD_CUSTOM_COMMAND(
            OUTPUT ${DOXYGEN_CS_OUTPUT_DIR}/html/index.html DEPENDS ${cs_source_files}
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cs
            COMMENT "Generating C# API documentation with Doxygen"
        )
        ADD_CUSTOM_TARGET(doxygen_cs_docs ALL DEPENDS ${DOXYGEN_CS_OUTPUT_DIR}/html/index.html)
        INSTALL(DIRECTORY ${DOXYGEN_CS_OUTPUT_DIR} DESTINATION ${DOCS_DIR}/cs COMPONENT doxygen)
    ENDIF(ENABLE_CSHARP)

    IF(ENABLE_JAVA)
        SET(DOXYGEN_JAVA_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/java")
        MAKE_DIRECTORY(${DOXYGEN_JAVA_OUTPUT_DIR})
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_java.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_java
        @ONLY)

        FILE(GLOB_RECURSE java_source_files ${LIBPKMN_BINARY_DIR}/lib/swig/java/org/nc/LibPKMN/*.java)
        ADD_CUSTOM_COMMAND(
            OUTPUT ${DOXYGEN_JAVA_OUTPUT_DIR}/html/index.html DEPENDS ${java_source_files}
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_java
            COMMENT "Generating Java API documentation with Doxygen"
        )
        ADD_CUSTOM_TARGET(doxygen_java_docs ALL DEPENDS ${DOXYGEN_JAVA_OUTPUT_DIR}/html/index.html)
        INSTALL(DIRECTORY ${DOXYGEN_JAVA_OUTPUT_DIR} DESTINATION ${DOCS_DIR}/java COMPONENT doxygen)
    ENDIF(ENABLE_JAVA)
ENDIF(ENABLE_DOXYGEN)
