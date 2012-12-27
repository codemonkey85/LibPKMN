FUNCTION(TEST_DATABASE_TABLE FIELD TABLE_NAME ENTRY VALUE DESIRED_DISPLAY_NAME)
    EXECUTE_PROCESS(
        COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db "SELECT ${FIELD} from ${TABLE_NAME} where ${ENTRY}=${VALUE}"
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
TEST_DATABASE_TABLE("display_name" "types" "Dark_mod" "0.0" "Psychic")
TEST_DATABASE_TABLE("display_name" "gen1_types" "Ice_mod" "0.5" "Ice")
TEST_DATABASE_TABLE("first_gen" "abilities" "display_name" "'Rivalry'" "4")
TEST_DATABASE_TABLE("display_name" "pokedex" "pokedex_num" "208" "Steelix")
TEST_DATABASE_TABLE("display_name" "gen1_pokedex" "pokedex_num" "25" "Pikachu")
TEST_DATABASE_TABLE("display_name" "gen2_pokedex" "pokedex_num" "152" "Chikorita")
TEST_DATABASE_TABLE("display_name" "gen3_pokedex" "pokedex_num" "315" "Roselia")
TEST_DATABASE_TABLE("satk_mod" "natures" "display_name" "'Rash'" "1.1")
TEST_DATABASE_TABLE("pkmn2_identifier" "evolutions" "pkmn1_identifier" "'charmeleon'" "charizard")
TEST_DATABASE_TABLE("in_gen4" "items" "display_name" "'BlackGlasses'" "1")

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.db
    DESTINATION ${PKG_DATA_DIR}
    COMPONENT "database"
)
ADD_SUBDIRECTORY(sample_teams)
