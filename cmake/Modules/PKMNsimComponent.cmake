#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

set(_enabled_components "" CACHE INTERNAL "" FORCE)
set(_disabled_components "" CACHE INTERNAL "" FORCE)

########################################################################
# Register a component into the system
#  - name the component string name
#  - var the global enable variable
#  - enb the default enable setting
#  - deps a list of dependencies
#  - dis the default disable setting
########################################################################
MACRO(REGISTER_COMPONENT name var enb deps dis)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Determining support for ${name}.")
    FOREACH(dep ${deps})
        MESSAGE(STATUS " - Dependency ${dep} = ${${dep}}")
    ENDFOREACH(dep)

    #setup the dependent option for this component
    INCLUDE(CMakeDependentOption)
    CMAKE_DEPENDENT_OPTION(${var} "enable ${name} support" ${enb} "${deps}" ${dis})

    #append the component into one of the lists
    IF(${var})
        MESSAGE(STATUS " - Enabling ${name}.")
        LIST(APPEND _enabled_components ${name})
    ELSE(${var})
        MESSAGE(STATUS " - Disabling ${name}.")
        LIST(APPEND _disabled_components ${name})
    ENDIF(${var})
    MESSAGE(STATUS " - Override with -D${var}=ON/OFF")

    #make components lists into global variables
    SET(_enabled_components ${_enabled_components} CACHE INTERNAL "" FORCE)
    SET(_disabled_components ${_disabled_components} CACHE INTERNAL "" FORCE)
ENDMACRO(REGISTER_COMPONENT)

#Print the summary of enabled/disabled components
FUNCTION(PRINT_COMPONENT_SUMMARY)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# PKMNsim - Enabled Components                        ")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_enabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# PKMNsim - Disabled Components                       ")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_disabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
ENDFUNCTION(PRINT_COMPONENT_SUMMARY)

