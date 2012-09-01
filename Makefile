all:
	g++ poketest.cpp lib/pokedex.cpp lib/base_pkmn.cpp lib/spec_pkmn.cpp -Iinclude -o poketest

clean:
	rm -rf poketest
