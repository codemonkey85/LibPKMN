#include <iostream>
#include <string>
#include <map>
#include <boost/assign.hpp>
#include <pkmnsim/base_pkmn.hpp>

std::map<std::string, base_pkmn> nat_pokedex = boost::assign::map_list_of
    ("bulbasaur", base_pkmn("Bulbasaur",1,"Seed", "Grass","Poison", "Overgrow","Chlorophyll","",
                            0.7,6.9, 0.875,0.125, 45,49,65,65,45, 0,0,0,1,0,0))
    ("charmander", base_pkmn("Charmander",4,"Lizard", "Fire","", "Blaze","Solar Power","",
                             0.6,8.5, 0.875,0.125, 39,52,43,60,50,65, 0,0,0,0,0,1))
    ("squirtle", base_pkmn("Squirtle",7,"Tiny Turtle", "Water","", "Torrent","Rain Dish","",
                           0.5,9.0, 0.875,0.125, 44,48,65,50,64,43, 0,0,1,0,0,0))
    ;
