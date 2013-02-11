SET(pkmnsim_bin_sources
    #search_type_combo.cpp
    team_analysis.cpp
)

FOREACH(bin_src ${pkmnsim_bin_sources})
    GET_FILENAME_COMPONENT(bin_name ${bin_src} NAME_WE)
    ADD_EXECUTABLE(${bin_name} ${bin_src})
    TARGET_LINK_LIBRARIES(${bin_name} pkmnsim boost_program_options sqlite3)
    INSTALL(TARGETS ${bin_name} RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT utilities)
ENDFOREACH(bin_src)
