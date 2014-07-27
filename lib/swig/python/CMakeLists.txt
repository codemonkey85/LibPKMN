#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")

#TODO: generate this list at CMake-time
SET(py_enum_files
    ${CMAKE_CURRENT_BINARY_DIR}/Abilities.py
    ${CMAKE_CURRENT_BINARY_DIR}/Egg_Groups.py
    ${CMAKE_CURRENT_BINARY_DIR}/Forms.py
    ${CMAKE_CURRENT_BINARY_DIR}/Genders.py
    ${CMAKE_CURRENT_BINARY_DIR}/Items.py
    ${CMAKE_CURRENT_BINARY_DIR}/Markings.py
    ${CMAKE_CURRENT_BINARY_DIR}/Move_Classes.py
    ${CMAKE_CURRENT_BINARY_DIR}/Moves.py
    ${CMAKE_CURRENT_BINARY_DIR}/Natures.py
    ${CMAKE_CURRENT_BINARY_DIR}/PokeBalls.py
    ${CMAKE_CURRENT_BINARY_DIR}/Ribbons.py
    ${CMAKE_CURRENT_BINARY_DIR}/Species.py
    ${CMAKE_CURRENT_BINARY_DIR}/Stats.py
    ${CMAKE_CURRENT_BINARY_DIR}/Types.py
    ${CMAKE_CURRENT_BINARY_DIR}/Version_Groups.py
    ${CMAKE_CURRENT_BINARY_DIR}/Versions.py
)

#Generate enums
ADD_CUSTOM_COMMAND(
    OUTPUT ${py_enum_files}
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=python --include-path=${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Python enums"
)
ADD_CUSTOM_TARGET(
    python_enums ALL
    DEPENDS ${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp
    DEPENDS ${py_enum_files}
)

INSTALL(
        FILES ${py_enum_files}
        DESTINATION ${LIBPKMN_PYTHON_DIR}/LibPKMN
        COMPONENT "LibPKMN_Python"
)

PYTHON_CHECK_MODULE(PyQt4 PyQt4 True PYQT4_FOUND)
PYTHON_CHECK_MODULE(SIP sip True SIP_FOUND)
MESSAGE(STATUS "") #Formatting

IF(PYQT4_FOUND AND SIP_FOUND)
    SET(IMPORT_QT4 "import qt4")
    ADD_SUBDIRECTORY(qt4)
ENDIF(PYQT4_FOUND AND SIP_FOUND)

INCLUDE(LibPKMNPython)
PYTHON_BUILD_SWIG_MODULE(libpkmn_python_swig LibPKMN)

ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(io)
