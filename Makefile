all:
	g++ poketest.cpp lib/pkmn_nature.cpp lib/globals.cpp lib/base_pkmn.cpp lib/spec_pkmn.cpp -Iinclude -o poketest

clean:
	rm -rf poketest
