#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Look for libraries and programs associated with different languages and
# register components accordingly.

FIND_PACKAGE(SWIG)
IF(SWIG_FOUND)
    FIND_PACKAGE(PythonLibs 2)

    FIND_PACKAGE(CSharp)
    FIND_PACKAGE(Java)
    FIND_PACKAGE(JNI)
ENDIF(SWIG_FOUND)

LIBPKMN_REGISTER_COMPONENT("LibPKMN - Python API" ENABLE_PYTHON ON
                           "ENABLE_LIBPKMN;SWIG_FOUND;PYTHONLIBS_FOUND" OFF)
LIBPKMN_REGISTER_COMPONENT("LibPKMN - C# API" ENABLE_CSHARP ON
                           "ENABLE_LIBPKMN;CSHARP_FOUND" OFF)
LIBPKMN_REGISTER_COMPONENT("LibPKMN - Java API" ENABLE_JAVA ON
                           "ENABLE_LIBPKMN;JAVA_FOUND;JNI_FOUND" OFF)

####################################################################
# Add subdirectories as appropriate based on enabled SWIG wrappers
####################################################################

IF(ENABLE_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_PYTHON)
IF(ENABLE_CSHARP)
    ADD_SUBDIRECTORY(cs)
ENDIF(ENABLE_CSHARP)
IF(ENABLE_JAVA)
    ADD_SUBDIRECTORY(java)
ENDIF(ENABLE_JAVA)
