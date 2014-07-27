#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")

PYTHON_CHECK_MODULE(PyQt4 PyQt4 True PYQT4_FOUND)
PYTHON_CHECK_MODULE(SIP sip True SIP_FOUND)
MESSAGE(STATUS "") #Formatting

IF(PYQT4_FOUND AND SIP_FOUND)
    SET(IMPORT_QT4 "import qt4")
    ADD_SUBDIRECTORY(qt4)
ENDIF(PYQT4_FOUND AND SIP_FOUND)

INCLUDE(LibPKMNPython)
PYTHON_BUILD_MODULE(libpkmn_python_swig LibPKMN)

ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(io)
