#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Look for libraries and programs associated with different languages and
# register components accordingly.

FIND_PACKAGE(SWIG)
IF(SWIG_FOUND)
    FIND_PACKAGE(PythonInterp)
    FIND_PACKAGE(PythonLibs)
    IF(PYTHONINTERP_FOUND)
        INCLUDE(${PKMNSIM_SOURCE_DIR}/cmake/Modules/PKMNsimPython.cmake)
        PYTHON_CHECK_MODULE("PySQLite3" sqlite3 True PYSQLITE3_FOUND)
    ENDIF(PYTHONINTERP_FOUND)
ENDIF(SWIG_FOUND)

REGISTER_COMPONENT("PyPKMNsim Library - Core" ENABLE_PYPKMNSIM_LIB ON
                   "ENABLE_LIBPKMNSIM;SWIG_FOUND;PYTHONINTERP_FOUND;PYTHONLIBS_FOUND;PYSQLITE3_FOUND" OFF)

####################################################################
# Add subdirectories as appropriate based on enabled SWIG wrappers
####################################################################
ADD_SUBDIRECTORY(pkmnsim)
ADD_SUBDIRECTORY(pokelib)
