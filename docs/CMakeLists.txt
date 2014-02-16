#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PACKAGE(Doxygen)

LIBPKMN_REGISTER_COMPONENT("Doxygen - C++ API Documentation" ENABLE_DOXYGEN ON "DOXYGEN_FOUND" OFF)

IF(ENABLE_DOXYGEN)
    #Generate the Doxygen configuration file
    SET(CMAKE_CURRENT_BINARY_DIR_DOXYGEN ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY)
    
    FILE(GLOB_RECURSE header_files ${CMAKE_SOURCE_DIR}/include/*.hpp)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR_DOXYGEN} DEPENDS ${header_files}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        COMMENT "Generating API documentation with Doxygen"
    )

    #make the doxygen generation a built-time dependency
    ADD_CUSTOM_TARGET(doxygen_docs ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR_DOXYGEN})
    INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR_DOXYGEN} DESTINATION ${DOCS_DIR}/cpp COMPONENT doxygen)
ENDIF(ENABLE_DOXYGEN)
