#include <iostream>
#include <pkmnsim/pokedex.hpp>
#include <string>
#include <map>
#include <pkmnsim/spec_pkmn.hpp>

int main()
{
    extern std::map<std::string, base_pkmn> nat_pokedex;

    spec_pkmn bulbasaur1 = spec_pkmn(nat_pokedex["bulbasaur"],100,0);
    std::cout << "Poketest compile" << std::endl;
}
