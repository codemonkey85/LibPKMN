FILE(GLOB pkmnsim_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")

FOREACH(header ${pkmnsim_headers})
    INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/pkmnsim COMPONENT headers)
ENDFOREACH(header)
