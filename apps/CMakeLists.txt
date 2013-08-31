#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

####################################################################
# Set which applications will build based on enabled library
# components
####################################################################
PKMNSIM_REGISTER_COMPONENT("Applications - C++ Command Line" ENABLE_CMD_CPP_APPS ON "ENABLE_LIBPKMNSIM" OFF)
PKMNSIM_REGISTER_COMPONENT("Applications - C++ Qt4" ENABLE_QT4_CPP_APPS ON "ENABLE_LIBPKMNSIM_QT4" OFF)

IF(ENABLE_CMD_CPP_APPS)
    ADD_SUBDIRECTORY(cpp)
ENDIF(ENABLE_CMD_CPP_APPS)

#IF(ENABLE_PYTHON_APPS)
#    INCLUDE(PKMNsimPython)
#    PYTHON_CHECK_MODULE("py2exe" py2exe True PY2EXE_FOUND)
#    REGISTER_COMPONENT("Windows Python Packaging" ENABLE_WIN_PYTHON_PKG ON "ENABLE_PYTHON_APPS;WIN7_PKG;PY2EXE_FOUND" OFF)
#    ADD_SUBDIRECTORY(python)
#ENDIF(ENABLE_PYTHON_APPS)
