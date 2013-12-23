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
ENDIF(SWIG_FOUND)

PKMNSIM_REGISTER_COMPONENT("PyPKMNsim - Python Bindings" ENABLE_PYPKMNSIM ON
                           "ENABLE_LIBPKMNSIM;SWIG_FOUND;PYTHONINTERP_FOUND;PYTHONLIBS_FOUND" OFF)
PKMNSIM_REGISTER_COMPONENT("PKMNsim# - C# Bindings" ENABLE_PKMNSIMSHARP ON
                           "ENABLE_LIBPKMNSIM;CSHARP_FOUND" OFF)

####################################################################
# Add subdirectories as appropriate based on enabled SWIG wrappers
####################################################################

#Read enum contents
FILE(STRINGS "${PKMNSIM_SOURCE_DIR}/include/pkmnsim/enums.hpp" ENUMS_HPP_CONTENTS)
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

IF(ENABLE_PYPKMNSIM)
    ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_PYPKMNSIM)
IF(ENABLE_PKMNSIMSHARP)
    ADD_SUBDIRECTORY(cs)
ENDIF(ENABLE_PKMNSIMSHARP)
