/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/analysis.hpp>

using namespace std;

//Base stat calculations

namespace pkmnsim
{
    //TODO: this only applies to Generation 3-5, add others
    int return_min_possible_stat(base_pkmn::sptr bpkmn, string stat, int level)
    {
        //IV = 0
        //EV = 0

        return int(ceil(((((2*(bpkmn->get_base_stats()[stat])) * level)/100) + 5) * 0.9));
    }

    //TODO: this only applies to Generation 3-5, add others
    int return_max_possible_stat(base_pkmn::sptr bpkmn, string stat, int level)
    {
        //IV = 31
        //EV = 255

        return int(ceil(((((31 + 2*(bpkmn->get_base_stats()[stat]) + 63.75) * level)/100) + 5) * 1.1));
    }

    bool is_stat_possible(base_pkmn::sptr bpkmn, int stat_value, string stat, int level)
    {
        return (stat_value > return_min_possible_stat(bpkmn,stat,level) and stat_value < return_max_possible_stat(bpkmn,stat,level));
    }
}
