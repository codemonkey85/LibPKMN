FUNCTION(TEST_DATABASE TABLE_NAME ENTRY VALUE DESIRED_DISPLAY_NAME)
    EXECUTE_PROCESS(
        COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db "SELECT display_name from ${TABLE_NAME} where ${ENTRY}=${VALUE}"
        OUTPUT_VARIABLE DISPLAY_NAME OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    IF("${DISPLAY_NAME}" STREQUAL "${DESIRED_DISPLAY_NAME}")
        MESSAGE(STATUS " * '${TABLE_NAME}': successful.")
    ELSE("${DISPLAY_NAME}" STREQUAL "${DESIRED_DISPLAY_NAME}")
        MESSAGE(STATUS " * '${TABLE_NAME}': Unsuccessful.")
        MESSAGE(FATAL_ERROR "Database import failed.")
    ENDIF("${DISPLAY_NAME}" STREQUAL "${DESIRED_DISPLAY_NAME}")
ENDFUNCTION()

FILE(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db")
#Read pkmnsim_db_dump into pkmnsim.db
MESSAGE(STATUS "Reading tables into database file.")
EXECUTE_PROCESS(
    COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db ".read ${CMAKE_CURRENT_SOURCE_DIR}/pkmnsim_db_dump"
    OUTPUT_VARIABLE SQLITE3_READ_OUTPUT OUTPUT_STRIP_TRAILING_WHITESPACE
)

#Test pkmnsim_db_dump read into pkmnsim.db
MESSAGE(STATUS "Testing tables.")
TEST_DATABASE("types" "Dark_mod" "0.0" "Psychic")
TEST_DATABASE("gen1_types" "Ice_mod" "0.5" "Ice")
TEST_DATABASE("pokedex" "pokedex_num" "208" "Steelix")
TEST_DATABASE("gen1_pokedex" "pokedex_num" "25" "Pikachu")
TEST_DATABASE("natures" "display_name" "'Hardy'" "Hardy")

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db
    DESTINATION ${PKG_DATA_DIR}
    COMPONENT "database"
)
ADD_SUBDIRECTORY(sample_teams)
