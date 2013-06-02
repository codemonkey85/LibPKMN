/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <boost/assign.hpp>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/dict.hpp>

using namespace pkmnsim;
using namespace std;

//Functions to be used for both command-line and GUI versions of Get Type Stats

struct stat_st
{
    string stat_name;
    string pkmn_name;
    int stat_value;

    stat_st(string sn): stat_name(sn), pkmn_name("Missingno."), stat_value(-1) {}
};

//The values are what the base_pkmn subclasses return in get_base_stats()
dict<string, string> get_stat_map(void)
{
    dict<string, string> stat_map = boost::assign::map_list_of
        ("HP","HP")
        ("Attack","ATK")
        ("Defense","DEF")
        ("Special Attack","SATK")
        ("Special Defense","SDEF")
        ("Special","SPCL")
        ("Speed","SPD");

    return stat_map;
}

void remove_unevolved_pokemon(vector<base_pkmn::sptr>& pkmn_vector)
{
    vector<int> to_erase; //Gathering vector indices to erase

    for(int i = 0; i < pkmn_vector.size(); i++)
    {
        if(not pkmn_vector[i]->is_fully_evolved())
        {
            to_erase.push_back(i);
        }
    }

    for(int j = to_erase.size()-1; j >= 0; j--)
    {
        pkmn_vector.erase(pkmn_vector.begin() + to_erase[j]);
    }
}

void sort_pokemon_by_stats(string type1, string type2, vector<stat_st>& highest_stats,
                           vector<stat_st>& lowest_stats, int gen, bool lax, bool evolved)
{
    highest_stats.push_back(stat_st("HP"));
    highest_stats.push_back(stat_st("Attack"));
    highest_stats.push_back(stat_st("Defense"));
    highest_stats.push_back(stat_st("Speed"));
    if(gen == 1) highest_stats.push_back(stat_st("Special"));
    else
    {
        highest_stats.push_back(stat_st("Special Attack"));
        highest_stats.push_back(stat_st("Special Defense"));
    }
    lowest_stats = highest_stats;
    vector<base_pkmn::sptr> pkmn_vector;

    //Get relevant Pokémon and determine validity of options
    get_pkmn_of_type(pkmn_vector, type1, type2, gen, lax);
    if(pkmn_vector.size() == 0)
    {
        cerr << "No Pokémon of specified type combination in Generation " << gen << ".\n";
        exit(EXIT_FAILURE);
    }
    if(evolved) remove_unevolved_pokemon(pkmn_vector);

    for(int i = 0; i < pkmn_vector.size(); i++)
    {
        string pkmn_name = pkmn_vector[i]->get_display_name();
        dict<string, int> stats = pkmn_vector[i]->get_base_stats();

        for(int j = 0; j < highest_stats.size(); j++)
        {
            string stat_name = get_stat_map()[highest_stats[j].stat_name];

            //TODO: account for case of stats being equal by appending pkmn_name strings
            if(highest_stats[j].pkmn_name == "Missingno." or highest_stats[j].stat_value < stats[stat_name])
            {
                highest_stats[j].pkmn_name = pkmn_name;
                highest_stats[j].stat_value = stats[stat_name];
            }
            if(lowest_stats[j].pkmn_name == "Missingno." or lowest_stats[j].stat_value > stats[stat_name])
            {
                lowest_stats[j].pkmn_name = pkmn_name;
                lowest_stats[j].stat_value = stats[stat_name];
            }
        }
    }

}
