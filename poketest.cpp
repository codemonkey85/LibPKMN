#include <iostream>
#include <pkmnsim/globals.hpp>
#include <string>
#include <map>
#include <pkmnsim/spec_pkmn.hpp>

int main()
{
    extern std::map<std::string, base_pkmn> nat_pokedex;

    spec_pkmn bulbasaur1 = spec_pkmn(nat_pokedex["bulbasaur"],100,-1);
    std::cout << "Base Pokemon info" << std::endl << std::endl;
    bulbasaur1.get_base_pkmn().print();
    std::cout << std::endl << "Specific Pokemon info" << std::endl << std::endl;
    bulbasaur1.print();
}
