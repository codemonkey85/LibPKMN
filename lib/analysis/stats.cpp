/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <stdexcept>
#include <ctime>

#include <boost/format.hpp>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/analysis/stats.hpp>

using namespace std;

namespace pkmn
{
    namespace analysis
    {
        unsigned int get_min_possible_stat(base_pokemon::sptr b_pkmn,
                                           std::string stat_name,
                                           unsigned int level,
                                           unsigned int gen)
        {
            pkmn::dict<std::string, unsigned int> stats = b_pkmn->get_base_stats();

            //Check inputs for errors
            if(not stats.has_key(stat_name)) return 0;
            if(gen < 1 or gen > 6) return 0;

            //Gen 1-2: IV = 0, EV = 0
            //Gen 3-5: IV = 0, EV = 0, disadvantageous nature
            if(gen == 1 or gen == 2)
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((stats["HP"]+50.0)*level)/50.0)+10.0);
                else
                    return (unsigned int)(floor((stats[stat_name]*level)/50.0)+5.0);
            }
            else
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((((2.0*stats["HP"])+100.0)
                           *level)+10.0)/100.0));
                else
                    return (unsigned int)(floor(((((2.0*stats[stat_name])*level)/100.0)+5.0)*0.9));
            }
        }

        unsigned int get_max_possible_stat(base_pokemon::sptr b_pkmn,
                                           std::string stat_name,
                                           unsigned int level,
                                           unsigned int gen)
        {
            pkmn::dict<std::string, unsigned int> stats = b_pkmn->get_base_stats();

            //Check inputs for errors
            if(stats.has_key(stat_name)) return -1;
            if(gen < 1 or gen > 6) return -1;

            //Gen 1-2: IV = 15, EV = 65535
            //Gen 3-5: IV = 31, EV = 255, advantageous nature
            if(gen == 1 or gen == 2)
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((((15.0+stats["HP"]+
                           (pow(65535.0,0.5)/8.0)+50.0)*level)/50.0)+10.0)));
                else
                    return (unsigned int)(floor((((15.0+stats[stat_name]+(pow(65535.0,0.5)/8.0))
                           *level)/50.0)+5.0));
            }
            else
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((31.0+(2.0*stats["HP"]+(255.0/4.0)+100.0)
                            *level)/100.0)+10.0));
                else
                    return (unsigned int)(floor((((31.0 + (2.0*stats[stat_name]+(255.0/4.0))
                            *level) / 100.0) + 5.0) * 1.1));
            }
        }

        bool is_stat_possible(base_pokemon::sptr b_pkmn, unsigned int stat_value,
                              std::string stat_name, unsigned int level, unsigned int gen)
        {
            return (stat_value > get_min_possible_stat(b_pkmn,stat_name,level,gen)
                   and stat_value < get_max_possible_stat(b_pkmn,stat_name,level,gen));
        }

        std::pair<unsigned int, unsigned int> get_stat_range(base_pokemon::sptr b_pkmn, std::string stat_name,
                                                             unsigned int level, unsigned int gen)
        {
            std::pair<unsigned int, unsigned int> stat_pair;

            stat_pair.first = get_min_possible_stat(b_pkmn, stat_name, level, gen);
            stat_pair.second = get_max_possible_stat(b_pkmn, stat_name, level, gen);

            return stat_pair;
        }
    } /* namespace analysis */
} /* namespace pkmn */