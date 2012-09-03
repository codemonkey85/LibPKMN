ADD_LIBRARY(pkmnsim SHARED base_pkmn.cpp globals.cpp pkmn_nature.cpp spec_pkmn.cpp)
INSTALL(TARGETS pkmnsim
    LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR} COMPONENT libraries
)
