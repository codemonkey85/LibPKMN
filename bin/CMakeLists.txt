#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pkmnsim_bin_sources
    get_type_stats.cpp
    team_analysis.cpp
)

FOREACH(bin_src ${pkmnsim_bin_sources})
    GET_FILENAME_COMPONENT(bin_name ${bin_src} NAME_WE)
    ADD_EXECUTABLE(${bin_name} ${bin_src})
    TARGET_LINK_LIBRARIES(${bin_name} pkmnsim ${Boost_LIBRARIES} ${SQLITE3_LIBRARY})
    INSTALL(TARGETS ${bin_name} RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT utilities)
ENDFOREACH(bin_src)
