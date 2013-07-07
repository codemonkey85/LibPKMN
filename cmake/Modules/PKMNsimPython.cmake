#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# Check for the existence of a python module:
# - desc a string description of the check
# - mod the name of the module to import
# - cmd an additional command to run
# - have the result variable to set
########################################################################
macro(PYTHON_CHECK_MODULE desc mod cmd have)
    message(STATUS "")
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} -c "
#########################################
try:
    import ${mod}
    assert ${cmd}
except ImportError, AssertionError: exit(-1)
except: pass
#########################################"
        RESULT_VARIABLE ${have}
    )
    if(${have} EQUAL 0)
        message(STATUS "Checking for Python module ${desc} - found")
        set(${have} TRUE)
    else(${have} EQUAL 0)
        message(STATUS "Checking for Python module ${desc} - not found")
        set(${have} FALSE)
    endif(${have} EQUAL 0)
endmacro(PYTHON_CHECK_MODULE)