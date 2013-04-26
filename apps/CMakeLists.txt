#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "Determining which applications to build.")
MESSAGE(STATUS "")
ADD_SUBDIRECTORY(get_type_stats)
ADD_SUBDIRECTORY(team_analysis)
MESSAGE(STATUS "")
