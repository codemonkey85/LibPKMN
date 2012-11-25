ADD_LIBRARY(pkmnsim SHARED
                base_pkmn.cpp
                pkmn_nature.cpp
                pkmn_types.cpp
                spec_pkmn.cpp
                stat_analysis.cpp
                sqlitecpp/Column.cpp
                sqlitecpp/Database.cpp
                sqlitecpp/Statement.cpp
                sqlitecpp/Transaction.cpp)
INSTALL(TARGETS pkmnsim
    LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR} COMPONENT libraries
)
