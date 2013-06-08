/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>
#include <stdexcept>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/analysis.hpp>

using namespace std;

//Base stat calculations

namespace pkmnsim
{
    int return_min_possible_stat(base_pkmn::sptr bpkmn, string stat, int level, int gen)
    {
        //Check inputs for errors
        if(not bpkmn->get_base_stats().has_key(stat)) throw runtime_error("Invalid stat specified.");
        if(gen < 1 or gen > 5) throw runtime_error("Invalid generation specified.");

        //Gen 1-2: IV = 0, EV = 0
        //Gen 3-5: IV = 0, EV = 0, disadvantageous nature
        if(stat == "HP")
        {
            if(gen == 1 or gen == 2) return int(floor((((bpkmn->get_base_stats()["HP"] + 50) * level) / 50) + 10));
            else return int(floor((((2 * bpkmn->get_base_stats()["HP"]) * level) / 100) + 10));
        }
        else
        {
            if(gen == 1 or gen == 2) return int(floor((((bpkmn->get_base_stats()[stat] * level) / 50) + 5)));
            else return int(floor(((((2 * bpkmn->get_base_stats()[stat]) * level) / 100) + 5) * 0.9));
        }
    }

    int return_max_possible_stat(base_pkmn::sptr bpkmn, string stat, int level, int gen)
    {
        //Check inputs for errors
        if(not bpkmn->get_base_stats().has_key(stat)) throw runtime_error("Invalid stat specified.");
        if(gen < 1 or gen > 5) throw runtime_error("Invalid generation specified.");

        //Gen 1-2: IV = 15, EV = 65535
        //Gen 3-5: IV = 31, EV = 255, advantageous nature
        if(gen == 1 or gen == 2)
        {
            if(stat == "HP")
                return int(floor((((15 + bpkmn->get_base_stats()["HP"] + (pow(65535.0, 0.5) / 8.0) + 50) * level) / 50) + 10));
            else
                return int(floor((((15 + bpkmn->get_base_stats()[stat] + (pow(65535.0, 0.5) / 8.0)) * level) / 50) + 5));
        }
        else
        {
            if(stat == "HP")
                return int(floor(((31 + (2 * bpkmn->get_base_stats()["HP"]) + (255 / 4)) / 100) + 10));
            else
                return int(floor(((((31 + (2 * bpkmn->get_base_stats()[stat]) + (255 / 4)) * level) / 100) + 5) * 1.1));
        }
    }

    bool is_stat_possible(base_pkmn::sptr bpkmn, int stat_value, string stat, int level, int gen)
    {
        return (stat_value > return_min_possible_stat(bpkmn,stat,level,gen) and stat_value < return_max_possible_stat(bpkmn,stat,level,gen));
    }
}
