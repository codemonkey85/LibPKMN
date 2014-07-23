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

#Generate Qt4 subclass wrappers
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AbilitiesComboBoxProxy.cpp
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBoxProxy.cpp
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NaturesComboBoxProxy.cpp
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/PokemonListComboBoxProxy.cpp
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/TypesComboBoxProxy.cpp
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/VersionsComboBoxProxy.cpp
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AbilitiesComboBox.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBox.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NaturesComboBox.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/PokemonListComboBox.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/TypesComboBox.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/VersionsComboBox.py
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_qt4_wrappers.py --qt4-include-dir=${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4 --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Qt4 wrapper classes"
)
ADD_CUSTOM_TARGET(
    qt4_wrappers ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AbilitiesComboBoxProxy.cpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBoxProxy.cpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NaturesComboBoxProxy.cpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/PokemonListComboBoxProxy.cpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/TypesComboBoxProxy.cpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/VersionsComboBoxProxy.cpp
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AbilitiesComboBox.py
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBox.py
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NaturesComboBox.py
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/PokemonListComboBox.py
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/TypesComboBox.py
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/VersionsComboBox.py
)

SET(qt4proxy_sources
    ${CMAKE_CURRENT_BINARY_DIR}/AbilitiesComboBoxProxy.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBoxProxy.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/NaturesComboBoxProxy.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PokemonListComboBoxProxy.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/TypesComboBoxProxy.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/VersionsComboBoxProxy.cpp
)

SET(qt4wrapper_sources
    ${CMAKE_CURRENT_BINARY_DIR}/AbilitiesComboBox.py
    ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBox.py
    ${CMAKE_CURRENT_BINARY_DIR}/NaturesComboBox.py
    ${CMAKE_CURRENT_BINARY_DIR}/PokemonListComboBox.py
    ${CMAKE_CURRENT_BINARY_DIR}/TypesComboBox.py
    ${CMAKE_CURRENT_BINARY_DIR}/VersionsComboBox.py
)

ADD_LIBRARY(qt4proxy STATIC ${qt4proxy_sources})
TARGET_LINK_LIBRARIES(qt4proxy ${qt4proxy_libraries})
ADD_DEPENDENCIES(qt4proxy qt4_wrappers)
IF(UNIX)
    SET_TARGET_PROPERTIES(qt4proxy PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF(UNIX)

INCLUDE(LibPKMNPython)
PYTHON_BUILD_MODULE(qt4_swig LibPKMN/qt4)
