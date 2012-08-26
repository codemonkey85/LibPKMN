#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/base_pkmn.hpp>

int main()
{
    std::map<std::string, base_pkmn> nat_pokedex;

    nat_pokedex["bulbasaur"] = base_pkmn("Bulbasaur", 1, "Seed", "Grass","Poison", "Overgrow","Chlorophyll", "",
                                         0.7,6.9, 0.875,0.125, 45,49,49,65,65,45, 0,0,0,1,0,0);
    nat_pokedex["charmander"] = base_pkmn("Charmander", 4, "Lizard", "Fire", "", "Blaze", "Solar Power", "",
                                          0.6,8.5, 0.875,0.125, 39,52,43,60,50,65, 0,0,0,0,0,1);
    nat_pokedex["squirtle"] = base_pkmn("Squirtle", 7, "Tiny Turtle", "Water", "", "Torrent", "Rain Dish", "",
                                        0.5,9.0, 0.875,0.125, 44,48,65,50,64,43, 0,0,1,0,0,0); //One type
    nat_pokedex["pidgey"] = base_pkmn("Pidgey", 16, "Tiny Bird", "Normal", "Flying", "Keen Eye", "Tangled Feet", "Big Pecks",
                                      0.3,1.8, 0.5,0.5, 40,45,40,35,35,56, 0,0,0,0,0,1); //Three potential abilities
    nat_pokedex["mew"] = base_pkmn("Mew", 151, "New Species", "Psychic", "", "Synchronize", "", "",
                                   0.4,4.0, 0.0,0.0, 100,100,100,100,100,100, 3,0,0,0,0,0); //One potential ability

    nat_pokedex["bulbasaur"].print();
    std::cout << std::endl;
    nat_pokedex["charmander"].print();
    std::cout << std::endl;
    nat_pokedex["squirtle"].print();
    std::cout << std::endl;
    nat_pokedex["pidgey"].print();
    std::cout << std::endl;
    nat_pokedex["mew"].print();
}
