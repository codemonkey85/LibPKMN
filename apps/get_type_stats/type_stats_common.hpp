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
