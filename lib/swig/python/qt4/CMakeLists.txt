#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

SET(qt4proxy_libraries
    pkmn
    ${PYTHON_LIBRARIES}
)

SET(qt4proxy_sources
    AbilitiesComboBoxProxy.cpp
    AboutMessageBoxProxy.cpp
    NaturesComboBoxProxy.cpp
    PokemonListComboBoxProxy.cpp
    TypesComboBoxProxy.cpp
    VersionsComboBoxProxy.cpp
)

ADD_LIBRARY(qt4proxy STATIC ${qt4proxy_sources})
TARGET_LINK_LIBRARIES(qt4proxy ${qt4proxy_libraries})
IF(UNIX)
    SET_TARGET_PROPERTIES(qt4proxy PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF(UNIX)

INCLUDE(LibPKMNPython)
PYTHON_BUILD_MODULE(qt4_swig LibPKMN/qt4)
