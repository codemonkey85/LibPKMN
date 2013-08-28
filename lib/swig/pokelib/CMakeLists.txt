#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

####################################################################
# Add subdirectories as appropriate based on enabled SWIG wrappers
####################################################################
IF(ENABLE_PYPKMNSIM_LIB)
    ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_PYPKMNSIM_LIB)
