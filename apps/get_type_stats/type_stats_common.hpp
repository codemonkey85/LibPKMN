/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TYPE_STATS_COMMON_HPP
#define INCLUDED_TYPE_STATS_COMMON_HPP

#include <boost/assign.hpp>
#include <string>
#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/types/dict.hpp>
#include <vector>

//Functions to be used for both command-line and GUI versions of Get Type Stats

class stat_st
{
    public:
        stat_st(std::string sn)
        {
            stat_name = sn;
            pkmn_name = "Missingno.";
            stat_value = -1;
        }

    std::string stat_name;
    std::string pkmn_name; //To check if initialized
    pkmnsim::base_pokemon::sptr b_pkmn;
    int stat_value;
};

//Prototypes
pkmnsim::dict<std::string, unsigned int> get_stat_map();
void remove_unevolved_pokemon(std::vector<pkmnsim::base_pokemon::sptr> &pkmn_vector);
int sort_pokemon_by_stats(std::string type1, std::string type2, std::vector<stat_st> &highest_stats,
                           std::vector<stat_st> &lowest_stats, int gen, bool lax, bool evolved);
#endif /* INCLUDED_TYPE_STATS_COMMON_HPP */
