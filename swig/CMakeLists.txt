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
ENDIF(SWIG_FOUND)

REGISTER_COMPONENT("PKMNsim Library - Python Wrapper" ENABLE_LIBPKMNSIM_PYTHON ON
                   "ENABLE_LIBPKMNSIM;SWIG_FOUND;PYTHONINTERP_FOUND;PYTHONLIBS_FOUND" OFF)

# Add subdirectories as appropriate
IF(ENABLE_LIBPKMNSIM_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_LIBPKMNSIM_PYTHON)
