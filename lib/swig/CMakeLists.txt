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

    FIND_PACKAGE(CSharp)
    FIND_PACKAGE(Java)
    FIND_PACKAGE(JNI)
ENDIF(SWIG_FOUND)

LIBPKMN_REGISTER_COMPONENT("LibPKMN - Python API" ENABLE_PYTHON ON
                           "ENABLE_LIBPKMN;SWIG_FOUND;PYTHONINTERP_FOUND;PYTHONLIBS_FOUND" OFF)
LIBPKMN_REGISTER_COMPONENT("LibPKMN - C# API" ENABLE_CSHARP ON
                           "ENABLE_LIBPKMN;CSHARP_FOUND" OFF)
LIBPKMN_REGISTER_COMPONENT("LibPKMN - Java API" ENABLE_JAVA ON
                           "ENABLE_LIBPKMN;JAVA_FOUND;JNI_FOUND" OFF)

####################################################################
# Add subdirectories as appropriate based on enabled SWIG wrappers
####################################################################

#Read enum contents
FILE(STRINGS "${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp" ENUMS_HPP_CONTENTS)
SET(ENUMS_HPP_CONTENTS "${ENUMS_HPP_CONTENTS}" CACHE STRING "enums.hpp contents")
FOREACH(line ${ENUMS_HPP_CONTENTS})
    STRING(REGEX MATCH "^    namespace [^FR]+" line ${line})
    IF(line)
        STRING(REGEX REPLACE "^    namespace " "" line ${line})
        STRING(REGEX REPLACE "//.*$" "" line "${line}")
        LIST(APPEND NAMESPACES ${line})
    ENDIF(line)
ENDFOREACH(line ${ENUMS_HPP_CONTENTS})
SET(NAMESPACES "${NAMESPACES}" CACHE STRING "enums.hpp namespaces")

IF(ENABLE_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_PYTHON)
IF(ENABLE_CSHARP)
    ADD_SUBDIRECTORY(cs)
ENDIF(ENABLE_CSHARP)
IF(ENABLE_JAVA)
    ADD_SUBDIRECTORY(java)
ENDIF(ENABLE_JAVA)