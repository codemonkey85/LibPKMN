ADD_LIBRARY(pkmnsim SHARED
                base_pkmn.cpp
                pkmn_nature.cpp
                spec_pkmn.cpp
                stat_analysis.cpp)
INSTALL(TARGETS pkmnsim
    LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR} COMPONENT libraries
)
