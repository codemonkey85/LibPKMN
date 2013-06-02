/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <boost/assign.hpp>
#include <string>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/dict.hpp>
#include <vector>

//Functions to be used for both command-line and GUI versions of Get Type Stats

class stat_st
{
    public:
        stat_st(std::string sn) {stat_name = sn;}

    std::string stat_name;
    std::string pkmn_name;
    int stat_value;
};

//Prototypes
pkmnsim::dict<std::string, std::string> get_stat_map(void);
void remove_unevolved_pokemon(std::vector<pkmnsim::base_pkmn::sptr>& pkmn_vector);
void sort_pokemon_by_stats(std::string type1, std::string type2, std::vector<stat_st>& highest_stats,
                           std::vector<stat_st>& lowest_stats, int gen, bool lax, bool evolved);
