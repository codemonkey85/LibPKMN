#include <iostream>
#include <string>
#include <map>
#include <utility>
#include "base_pkmn"

map<std::string, base_pkmn> nat_pokedex;

nat_pokedex["bulbasaur"] = base_pkmn("Bulbasaur",1,"Seed","Grass","Poison",1.0,10.0,0.5,0.5,10,10,10,10,10,10,1,1,1,1,1,1);

int main()
{
    std::cout << nat_pokedex["bulbasaur"] << std::endl;
}
