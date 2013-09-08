/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <string>
#include <vector>

#include <boost/assign.hpp>

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/dict.hpp>

#include "type_stats_common.hpp"

using namespace pkmnsim;
using namespace std;

//The values are what the base_pokemon subclasses return in get_base_stats()
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

void remove_unevolved_pokemon(vector<base_pokemon::sptr>& pkmn_vector)
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

int sort_pokemon_by_stats(string type1, string type2, vector<stat_st>& highest_stats,
                           vector<stat_st>& lowest_stats, int gen, bool lax, bool evolved)
{
    highest_stats.clear();
    lowest_stats.clear();

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
    vector<base_pokemon::sptr> pkmn_vector;

    //Get relevant Pokémon and determine validity of options
    get_pkmn_of_type(pkmn_vector, type1, type2, gen, lax);
    if(pkmn_vector.size() == 0) return 1;
    if(evolved) remove_unevolved_pokemon(pkmn_vector);

    for(unsigned int i = 0; i < pkmn_vector.size(); i++)
    {
        string pkmn_name = pkmn_vector[i]->get_species_name();
        dict<string, int> stats = pkmn_vector[i]->get_base_stats();

        for(unsigned int j = 0; j < highest_stats.size(); j++)
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

    return 0;
}

int sort_pokemon_by_stats(string type1, string type2, vector<stat_st2>& highest_stats,
                           vector<stat_st2>& lowest_stats, int gen, bool lax, bool evolved)
{
    highest_stats.clear();
    lowest_stats.clear();

    highest_stats.push_back(stat_st2("HP"));
    highest_stats.push_back(stat_st2("Attack"));
    highest_stats.push_back(stat_st2("Defense"));
    highest_stats.push_back(stat_st2("Speed"));
    if(gen == 1) highest_stats.push_back(stat_st2("Special"));
    else
    {
        highest_stats.push_back(stat_st2("Special Attack"));
        highest_stats.push_back(stat_st2("Special Defense"));
    }
    lowest_stats = highest_stats;
    vector<base_pokemon::sptr> pkmn_vector;

    //Get relevant Pokémon and determine validity of options
    get_pkmn_of_type(pkmn_vector, type1, type2, gen, lax);
    if(pkmn_vector.size() == 0) return 1;
    if(evolved) remove_unevolved_pokemon(pkmn_vector);

    for(unsigned int i = 0; i < pkmn_vector.size(); i++)
    {
        string pkmn_name = pkmn_vector[i]->get_species_name();
        dict<string, int> stats = pkmn_vector[i]->get_base_stats();

        for(unsigned int j = 0; j < highest_stats.size(); j++)
        {
            string stat_name = get_stat_map()[highest_stats[j].stat_name];

            //TODO: account for case of stats being equal by appending pkmn_name strings
            if(highest_stats[j].pkmn_name == "Missingno." or highest_stats[j].stat_value < stats[stat_name])
            {
                highest_stats[j].b_pkmn = pkmn_vector[i];
                highest_stats[j].pkmn_name = pkmn_name;
                highest_stats[j].stat_value = stats[stat_name];
            }
            if(lowest_stats[j].pkmn_name == "Missingno." or lowest_stats[j].stat_value > stats[stat_name])
            {
                lowest_stats[j].b_pkmn = pkmn_vector[i];
                lowest_stats[j].pkmn_name = pkmn_name;
                lowest_stats[j].stat_value = stats[stat_name];
            }
        }
    }

    return 0;
}
