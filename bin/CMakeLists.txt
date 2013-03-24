#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "Determining which applications to build.")
MESSAGE(STATUS " * Get Type Stats")
MESSAGE(STATUS "   * Boost found: ${Boost_FOUND}")
MESSAGE(STATUS "   * SQLite3 found: ${SQLITE3_FOUND}")
IF(Boost_FOUND AND SQLITE3_FOUND)
    MESSAGE(STATUS "   * All dependencies found. Will build.")
    SET(pkmnsim_bin_srcs get_type_stats.cpp)
ENDIF(Boost_FOUND AND SQLITE3_FOUND)

MESSAGE(STATUS " * Team Analysis")
MESSAGE(STATUS "   * Boost found: ${Boost_FOUND}")
MESSAGE(STATUS "   * SQLite3 found: ${SQLITE3_FOUND}")
IF(Boost_FOUND AND SQLITE3_FOUND)
    MESSAGE(STATUS "   * All dependencies found. Will build.")
    SET(pkmnsim_bin_srcs ${pkmnsim_bin_srcs} team_analysis.cpp)
ENDIF(Boost_FOUND AND SQLITE3_FOUND)

MESSAGE(STATUS "")

FOREACH(bin_src ${pkmnsim_bin_srcs})
    GET_FILENAME_COMPONENT(bin_name ${bin_src} NAME_WE)
    ADD_EXECUTABLE(${bin_name} ${bin_src})
    TARGET_LINK_LIBRARIES(${bin_name} pkmnsim ${Boost_LIBRARIES} ${SQLITE3_LIBRARY})
    INSTALL(TARGETS ${bin_name} RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT utilities)
ENDFOREACH(bin_src)
