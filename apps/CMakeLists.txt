#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REGISTER_COMPONENT("Applications - C++ Command Line" ENABLE_CMD_CPP_APPS ON "ENABLE_LIBPKMNSIM" OFF)
REGISTER_COMPONENT("Applications - C++ Qt4" ENABLE_QT4_CPP_APPS ON "ENABLE_LIBPKMNSIM_QT4" OFF)

IF(ENABLE_CMD_CPP_APPS)
    ADD_SUBDIRECTORY(cpp)
ENDIF(ENABLE_CMD_CPP_APPS)
