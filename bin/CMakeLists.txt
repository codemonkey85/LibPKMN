FILE(COPY sample_teams DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ADD_EXECUTABLE(analyze_pkmn_team analyze_pkmn_team.cpp)
TARGET_LINK_LIBRARIES(analyze_pkmn_team pkmnsim)
