#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# Find first instance of "value" after line number "first_after"
# Assumes ${LINE_NUMBER} is initialized
########################################################################
macro(GET_LINE_NUMBER value filename)
    IF(WIN32)
        FIND_PROGRAM(FINDSTR_EXECUTABLE findstr ENV PATH)
        EXECUTE_PROCESS(
            COMMAND ${FINDSTR_EXECUTABLE} /N "${value}" "${filename}"
            OUTPUT_VARIABLE line_num_list
        )
        # RESULT_VARIABLE gets line, which has line number and line
        STRING(REPLACE ":" ";" line_num_list ${line_num_list})
        LIST(GET line_num_list 0 LINE_NUMBER)
    ENDIF(WIN32)
endmacro(GET_LINE_NUMBER)