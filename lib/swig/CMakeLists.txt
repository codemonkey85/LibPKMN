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
    FIND_PACKAGE(PythonLibs 2)
    IF(PYTHONINTERP_FOUND)
        INCLUDE(PKMNsimPython)
        PYTHON_CHECK_MODULE("PyQt4" PyQt4 True PYQT4_FOUND)
    ENDIF(PYTHONINTERP_FOUND)
ENDIF(SWIG_FOUND)

PKMNSIM_REGISTER_COMPONENT("PyPKMNsim - Python Bindings" ENABLE_PYPKMNSIM ON
                           "ENABLE_LIBRARY;SWIG_FOUND;PYTHONINTERP_FOUND;PYTHONLIBS_FOUND;PYQT4_FOUND" OFF)

####################################################################
# Add subdirectories as appropriate based on enabled SWIG wrappers
####################################################################
IF(ENABLE_PYPKMNSIM)
    ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_PYPKMNSIM)
