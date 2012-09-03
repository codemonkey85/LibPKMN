#include <iostream>
#include <string>
#include <map>
#include <boost/assign.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/globals.hpp>

std::map<std::string, base_pkmn> nat_pokedex = boost::assign::map_list_of
    ("bulbasaur", base_pkmn("Bulbasaur",1,"Seed", "Grass","Poison", "Overgrow","Chlorophyll","",
                            0.7,6.9, 0.875,0.125, 45,49,49,65,65,45, 0,0,0,1,0,0, 64)) //RBG,FRLG starter
    ("charmander", base_pkmn("Charmander",4,"Lizard", "Fire","", "Blaze","Solar Power","",
                             0.6,8.5, 0.875,0.125, 39,52,43,60,50,65, 0,0,0,0,0,1, 62)) //RBG,FRLG starter
    ("squirtle", base_pkmn("Squirtle",7,"Tiny Turtle", "Water","", "Torrent","Rain Dish","",
                           0.5,9.0, 0.875,0.125, 44,48,65,50,64,43, 0,0,1,0,0,0, 63)) //RBG,FRLG starter
    ("pikachu", base_pkmn("Pikachu",25,"Mouse", "Electric","", "Static","Lightningrod","",
                          0.4,6.0, 0.5,0.5, 35,55,30,50,40,90, 0,0,0,0,0,2, 82)) //Y starter
    ("chikorita", base_pkmn("Chikorita",152,"Leaf", "Grass","", "Overgrow","Leaf Guard","",
                            0.9,6.4, 0.875,0.125, 45,49,65,49,65,45, 0,0,0,0,1,0, 64)) //GSC,HGSS starter
    ("cyndaquil", base_pkmn("Cyndaquil",155,"Fire Mouse", "Fire","", "Blaze","Flash Fire","",
                            0.5,7.9, 0.875,0.125, 39,52,43,60,50,65, 0,0,0,0,0,1, 65)) //GSC,HGSS starter
    ("totodile", base_pkmn("Totodile",158,"Big Jaw", "Water","", "Torrent","Sheer Force","",
                           0.6,9.5, 0.875,0.125, 50,65,64,44,48,43, 0,1,0,0,0,0, 66)) //GSC,HGSS starter
    ("treecko", base_pkmn("Treecko",252,"Wood Gecko", "Grass","", "Overgrow","Unburden","",
                          0.5,5.0, 0.875,0.125, 40,45,35,65,55,70, 0,0,0,0,0,1, 65)) //RSE starter
    ("torchic", base_pkmn("Torchic",255,"Chick", "Fire","", "Blaze","Speed Boost","",
                          0.4,2.5, 0.875,0.125, 45,60,40,70,50,45, 0,0,0,1,0,0, 65)) //RSE starter
    ("mudkip", base_pkmn("Mudkip",258,"Mud Fish", "Water","", "Torrent","Damp","",
                         0.4,7.6, 0.875,0.125, 50,70,50,50,50,40, 0,1,0,0,0,0, 65)) //RSE starter
    ("turtwig", base_pkmn("Turtwig",387,"Tiny Leaf", "Grass","", "Overgrow","Shell Armor","",
                          0.4,10.2, 0.875,0.125, 55,68,64,45,55,31, 0,1,0,0,0,0, 64)) //DPP starter
    ("chimchar", base_pkmn("Chimchar",390,"Chimp", "Fire","", "Blaze","Iron Fist","",
                           0.5,6.2, 0.875,0.125, 44,58,44,58,44,61, 0,0,0,0,0,1, 65)) //DPP starter
    ("piplup", base_pkmn("Piplup",393,"Penguin", "Water","", "Torrent","Defiant","",
                         0.4,5.2, 0.875,0.125, 53,51,53,61,56,40, 0,0,0,1,0,0, 66)) //DPP starter
    ("snivy", base_pkmn("Snivy",495,"Grass Snake", "Grass","", "Overgrow","Contrary","",
                        0.6,8.1, 0.875,0.125, 45,45,55,45,55,63, 0,0,0,0,0,1, 28)) //BW,B2W2 starter
    ("tepig", base_pkmn("Tepig",498,"Fire Pig", "Fire","", "Blaze","Thick Fat","",
                        0.5,9.9, 0.875,0.125, 65,63,45,45,45,45, 1,0,0,0,0,0, 28)) //BW,B2W2 starter
    ("oshawatt", base_pkmn("Oshawatt",501,"Sea Otter", "Water","", "Torrent","Shell Armor","",
                           0.5,5.9, 0.875,0.125, 55,55,45,63,45,45, 0,0,0,1,0,0, 28)) //BW,B2W2 starter
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
