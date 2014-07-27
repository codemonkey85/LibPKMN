#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

SET(qt4proxy_libraries
    pkmn
    ${PYTHON_LIBRARIES}
)

MACRO(GENERATE_QT4_WRAPPER_FILES subclass)
    LIST(APPEND qt4subclass_headers ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/${subclass}.hpp)
    LIST(APPEND qt4proxy_sources ${CMAKE_CURRENT_BINARY_DIR}/${subclass}Proxy.cpp)
    LIST(APPEND qt4proxy_headers ${CMAKE_CURRENT_BINARY_DIR}/${subclass}Proxy.hpp)
    LIST(APPEND qt4wrapper_sources ${CMAKE_CURRENT_BINARY_DIR}/${subclass}.py)
ENDMACRO(GENERATE_QT4_WRAPPER_FILES)

GENERATE_QT4_WRAPPER_FILES("AbilitiesComboBox")
GENERATE_QT4_WRAPPER_FILES("AboutMessageBox")
GENERATE_QT4_WRAPPER_FILES("NaturesComboBox")
GENERATE_QT4_WRAPPER_FILES("PokemonListComboBox")
GENERATE_QT4_WRAPPER_FILES("TypesComboBox")
GENERATE_QT4_WRAPPER_FILES("VersionsComboBox")

#Generate Qt4 subclass wrappers
ADD_CUSTOM_COMMAND(
    OUTPUT ${qt4proxy_sources}
    OUTPUT ${qt4proxy_headers}
    OUTPUT ${qt4wrapper_sources}
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_qt4_wrappers.py --qt4-include-dir=${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4 --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Qt4 wrapper classes"
)
ADD_CUSTOM_TARGET(
    qt4_wrappers ALL
    DEPENDS ${qt4subclass_headers}
    DEPENDS ${qt4proxy_sources}
    DEPENDS ${qt4proxy_headers}
    DEPENDS ${qt4wrapper_sources}
)

ADD_LIBRARY(qt4proxy STATIC ${qt4proxy_sources})
TARGET_LINK_LIBRARIES(qt4proxy ${qt4proxy_libraries})
ADD_DEPENDENCIES(qt4proxy qt4_wrappers)
IF(UNIX)
    SET_TARGET_PROPERTIES(qt4proxy PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF(UNIX)

INCLUDE(LibPKMNPython)
PYTHON_BUILD_SWIG_MODULE(qt4_swig LibPKMN/qt4)

INSTALL(
    FILES ${qt4wrapper_sources}
    DESTINATION ${LIBPKMN_PYTHON_DIR}/LibPKMN/qt4
    COMPONENT "LibPKMN_Python"
)
