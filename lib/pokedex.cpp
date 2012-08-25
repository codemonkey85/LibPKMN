#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/base_pkmn.hpp>


int main()
{
    std::map<std::string, base_pkmn> nat_pokedex;

    nat_pokedex["bulbasaur"] = base_pkmn("Bulbasaur",1,"Seed","Grass","Poison","Overgrow","Chlorophyll",1.0,10.0,0.5,0.5,10,10,10,10,10,10,1,1,1,1,1,1);
    std::cout << "Successful Pokedex compile." << std::endl;
}
