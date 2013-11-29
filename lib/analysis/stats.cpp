/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>
#include <stdexcept>
#include <time.h>

#include <boost/format.hpp>

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/analysis/stats.hpp>

#include "../SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmnsim
{
    namespace analysis
    {
        unsigned int get_min_possible_stat(base_pokemon::sptr b_pkmn, unsigned int stat, unsigned int level, unsigned int gen)
        {
            //Check inputs for errors
            if(not b_pkmn->get_base_stats().has_key(stat)) return -1;
            if(gen < 1 or gen > 5) return -1;

            //Gen 1-2: IV = 0, EV = 0
            //Gen 3-5: IV = 0, EV = 0, disadvantageous nature
            dict<unsigned int, unsigned int> stats = b_pkmn->get_base_stats();
            if(gen == 1 or gen == 2)
            {
                if(stat == Stats::HP)
                    return (unsigned int)(floor(((stats[Stats::HP]+50.0)*level)/50.0)+10.0);
                else
                    return (unsigned int)(floor((stats[stat]*level)/50.0)+5.0);
            }
            else
            {
                if(stat == Stats::HP)
                    return (unsigned int)(floor(((((2.0*stats[Stats::HP])+100.0)*level)+10.0)/100.0));
                else
                    return (unsigned int)(floor(((((2.0*stats[stat])*level)/100.0)+5.0)*0.9));
            }
        }

        unsigned int get_max_possible_stat(base_pokemon::sptr b_pkmn, unsigned int stat, unsigned int level, unsigned int gen)
        {
            //Check inputs for errors
            if(not b_pkmn->get_base_stats().has_key(stat)) return -1;
            if(gen < 1 or gen > 5) return -1;

            //Gen 1-2: IV = 15, EV = 65535
            //Gen 3-5: IV = 31, EV = 255, advantageous nature
            dict<unsigned int, unsigned int> stats = b_pkmn->get_base_stats();
            if(gen == 1 or gen == 2)
            {
                if(stat == Stats::HP)
                    return (unsigned int)(floor(((((15.0+stats[Stats::HP]+(pow(65535.0,0.5)/8.0)+50.0)*level)/50.0)+10.0)));
                else
                    return (unsigned int)(floor((((15.0+stats[stat]+(pow(65535.0,0.5)/8.0))*level)/50.0)+5.0));
            }
            else
            {
                if(stat == Stats::HP)
                    return (unsigned int)(floor(((31.0+(2.0*stats[stat]+(255.0/4.0)+100.0)*level)/100.0)+10.0));
                else
                    return (unsigned int)(floor((((31.0 + (2.0*stats[stat]+(255.0/4.0)) * level) / 100.0) + 5.0) * 1.1));
            }
        }

        bool is_stat_possible(base_pokemon::sptr b_pkmn, unsigned int stat_value, unsigned int stat, unsigned int level, unsigned int gen)
        {
            return (stat_value > get_min_possible_stat(b_pkmn,stat,level,gen) and stat_value < get_max_possible_stat(b_pkmn,stat,level,gen));
        }

        void get_stat_range(base_pokemon::sptr b_pkmn, unsigned int stat, unsigned int level, unsigned int gen, vector<unsigned int> &stat_vec)
        {
            stat_vec.clear();

            stat_vec.push_back(get_min_possible_stat(b_pkmn, stat, level, gen));
            stat_vec.push_back(get_max_possible_stat(b_pkmn, stat, level, gen));
        }
    } /* namespace analysis */
} /* namespace pkmnsim */