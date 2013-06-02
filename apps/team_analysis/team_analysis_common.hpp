/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>
#include <string>
#include <vector>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/dict.hpp>

using namespace pkmnsim;
using namespace std;

typedef map<string, int>::iterator si_iter;
typedef map<string, double>::iterator sd_iter;

map<string, int> get_type_overlaps(vector<base_pkmn::sptr> pkmn_team, vector<string> type_list)
{
    map<string, int> nums; //Key = type, val = number of Pokémon with that type
    for(int i = 0; i < type_list.size(); i++) nums[type_list[i]] = 0;

    //Generate vector with number of occurrences of each type
    for(int i = 0; i < pkmn_team.size(); i++)
    {
        dict<int, string> types = pkmn_team[i]->get_types();
        nums[types[0]]++;
        if(types[1] != "None") nums[types[1]]++;
    }

    //Delete map entries for non-overlapping types
    for(int i = 0; i < type_list.size(); i++)
    {
        if(nums[type_list[i]] < 2) nums.erase(type_list[i]);
    }

    return nums;
}

void trim_effectiveness_maps(map<string, int> &super_effective_map, map<string, int> &not_very_effective_map)
{
    for(si_iter i = super_effective_map.begin(); i != super_effective_map.end(); ++i)
    {
        string type_name = i->first;
        if(i->second > not_very_effective_map[type_name]) not_very_effective_map[type_name] = 0;
        else if(i->second < not_very_effective_map[type_name]) i->second = 0;
        else //Equal number, neither are a trend
        {
            i->second = 0;
            not_very_effective_map[type_name] = 0;
        }
    }
}
