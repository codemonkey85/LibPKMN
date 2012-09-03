#include <iostream>
#include <string>
#include <map>
#include <boost/assign.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/globals.hpp>

std::map<std::string, base_pkmn> nat_pokedex = boost::assign::map_list_of
    ("bulbasaur", base_pkmn("Bulbasaur",1,"Seed", "Grass","Poison", "Overgrow","Chlorophyll","",
                            0.7,6.9, 0.875,0.125, 45,49,49,65,65,45, 0,0,0,1,0,0, 64))
    ("charmander", base_pkmn("Charmander",4,"Lizard", "Fire","", "Blaze","Solar Power","",
                             0.6,8.5, 0.875,0.125, 39,52,43,60,50,65, 0,0,0,0,0,1, 62))
    ("squirtle", base_pkmn("Squirtle",7,"Tiny Turtle", "Water","", "Torrent","Rain Dish","",
                           0.5,9.0, 0.875,0.125, 44,48,65,50,64,43, 0,0,1,0,0,0, 63))
    ;

std::map<std::string, pkmn_nature> pkmn_nature_map = boost::assign::map_list_of
    ("hardy", pkmn_nature("Hardy",     1.0,1.0,1.0,1.0,1.0))
    ("lonely", pkmn_nature("Lonely",   1.1,0.9,1.0,1.0,1.0))
    ("brave", pkmn_nature("Brave",     1.1,1.0,0.9,1.0,1.0))
    ("adamant", pkmn_nature("Adamant", 1.1,1.0,1.0,0.9,1.0))
    ("naughty", pkmn_nature("Naughty", 1.1,1.0,1.0,1.0,0.9))
    ("bold", pkmn_nature("Bold",       0.9,1.1,1.0,1.0,1.0))
    ("docile", pkmn_nature("Docile",   1.0,1.0,1.0,1.0,1.0))
    ("relaxed", pkmn_nature("Relaxed", 1.0,1.1,0.9,1.0,1.0))
    ("impish", pkmn_nature("Impish",   1.0,1.1,1.0,0.9,1.0))
    ("lax", pkmn_nature("Lax",         1.0,1.1,1.0,1.0,0.9))
    ("timid", pkmn_nature("Timid",     0.9,1.0,1.1,1.0,1.0))
    ("serious", pkmn_nature("Serious", 1.0,0.9,1.1,1.0,1.0))
    ("jolly", pkmn_nature("Jolly",     1.0,1.0,1.1,0.9,1.0))
    ("naive", pkmn_nature("Naive",     1.0,1.0,1.1,1.0,0.9))
    ("modest", pkmn_nature("Modest",   0.9,1.0,1.0,1.1,1.0))
    ("mild", pkmn_nature("Mild",       1.0,0.9,1.0,1.0,1.1))
    ("quiet", pkmn_nature("Quiet",     1.0,1.0,0.9,1.1,1.0))
    ("bashful", pkmn_nature("Bashful", 1.0,1.0,1.0,1.0,1.0))
    ("rash", pkmn_nature("Rash",       1.0,1.0,1.0,1.1,0.9))
    ("calm", pkmn_nature("Calm",       0.9,1.0,1.0,1.0,1.1))
    ("gentle", pkmn_nature("Gentle",   1.0,0.9,1.0,1.0,1.1))
    ("sassy", pkmn_nature("Sassy",     1.0,1.0,0.9,1.0,1.1))
    ("careful", pkmn_nature("Careful", 1.0,1.0,1.0,0.9,1.1))
    ("quirky", pkmn_nature("Quirky",   1.0,1.0,1.0,1.0,1.0))
    ;
