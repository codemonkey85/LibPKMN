LIB_FILES = lib/pkmn_nature.cpp lib/globals.cpp lib/spec_pkmn.cpp lib/base_pkmn.cpp
INCLUDES = -Iinclude
LINKS = -lboost_program_options

all:
	g++ poketest.cpp $(LIB_FILES) $(INCLUDES) $(LINKS) -o poketest

clean:
	rm -rf poketest
