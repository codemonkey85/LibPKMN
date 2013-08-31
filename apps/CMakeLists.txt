#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

####################################################################
# Set which applications will build based on enabled library
# components
####################################################################
PKMNSIM_REGISTER_COMPONENT("Applications - C++ Command Line" ENABLE_CMD_CPP_APPS ON "ENABLE_LIBPKMNSIM" OFF)
PKMNSIM_REGISTER_COMPONENT("Applications - C++ Qt4" ENABLE_QT4_CPP_APPS ON "ENABLE_LIBPKMNSIM_QT4" OFF)

IF(ENABLE_CMD_CPP_APPS)
    ADD_SUBDIRECTORY(get_type_stats)
    ADD_SUBDIRECTORY(team_analysis)
    ADD_SUBDIRECTORY(damage_calculator)
ENDIF(ENABLE_CMD_CPP_APPS)
