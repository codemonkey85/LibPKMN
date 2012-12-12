FILE(GLOB sqlitecpp_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

FOREACH(header ${sqlitecpp_headers})
    INSTALL(FILES ${header} DESTINATION ${INCLUDE_DIR}/sqlitecpp COMPONENT headers)
ENDFOREACH(header)
