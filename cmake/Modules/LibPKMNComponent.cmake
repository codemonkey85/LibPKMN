#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

set(_libpkmn_enabled_components "" CACHE INTERNAL "" FORCE)
set(_libpkmn_disabled_components "" CACHE INTERNAL "" FORCE)

########################################################################
# Register a component into the system
#  - name the component string name
#  - var the global enable variable
#  - enb the default enable setting
#  - deps a list of dependencies
#  - dis the default disable setting
########################################################################
MACRO(LIBPKMN_REGISTER_COMPONENT name var enb deps dis)
    IF(NOT LIBPKMN_USED_AS_SUBMODULE)
        MESSAGE(STATUS "")
        MESSAGE(STATUS "Determining support for ${name}.")
        FOREACH(dep ${deps})
            MESSAGE(STATUS " - Dependency ${dep} = ${${dep}}")
        ENDFOREACH(dep)
    ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)

    #setup the dependent option for this component
    INCLUDE(CMakeDependentOption)
    CMAKE_DEPENDENT_OPTION(${var} "enable ${name} support" ${enb} "${deps}" ${dis})

    #append the component into one of the lists
    IF(${var})
        IF(NOT LIBPKMN_USED_AS_SUBMODULE)
            MESSAGE(STATUS " - Enabling ${name}.")
        ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)
        LIST(APPEND _libpkmn_enabled_components ${name})
    ELSE(${var})
        IF(NOT LIBPKMN_USED_AS_SUBMODULE)
            MESSAGE(STATUS " - Disabling ${name}.")
        ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)
        LIST(APPEND _libpkmn_disabled_components ${name})
    ENDIF(${var})
    IF(NOT LIBPKMN_USED_AS_SUBMODULE)
        MESSAGE(STATUS " - Override with -D${var}=ON/OFF")
    ENDIF(NOT LIBPKMN_USED_AS_SUBMODULE)

    #make components lists into global variables
    SET(_libpkmn_enabled_components ${_libpkmn_enabled_components} CACHE INTERNAL "" FORCE)
    SET(_libpkmn_disabled_components ${_libpkmn_disabled_components} CACHE INTERNAL "" FORCE)
ENDMACRO(LIBPKMN_REGISTER_COMPONENT)

#Print the summary of enabled/disabled components
FUNCTION(LIBPKMN_PRINT_COMPONENT_SUMMARY)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# LibPKMN - Enabled Components                        ")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_libpkmn_enabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# LibPKMN - Disabled Components                       ")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_libpkmn_disabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
ENDFUNCTION(LIBPKMN_PRINT_COMPONENT_SUMMARY)

