/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "base_pkmn_gen2impl.hpp"
#include <boost/format.hpp>
#include "sqlitecpp/SQLiteCPP.h"
#include <stdio.h>

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen2impl::base_pkmn_gen2impl(string identifier, SQLite::Database *db,
                                           bool query_moves): base_pkmn(identifier, 2, db, query_moves)
    {
        string query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=4")
                                         % pkmn_id);
        baseSATK = db->execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=5")
                                         % pkmn_id);
        baseSDEF = db->execAndGet(query_string.c_str(), identifier); 

        //Gender rates
        map<int, double> gender_val_map; //Double is percentage male
        gender_val_map[0] = 1.0;
        gender_val_map[1] = 0.875;
        gender_val_map[2] = 0.75;
        gender_val_map[4] = 0.5;
        gender_val_map[6] = 0.25;
        gender_val_map[8] = 0.0;

        query_string = str(boost::format("SELECT gender_rate FROM pokemon_species WHERE id=%d")
                                         % species_id);
        int gender_val = db->execAndGet(query_string.c_str(), identifier);

        if(gender_val == -1)
        {
            chance_male = 0.0;
            chance_female = 0.0;
        }
        else
        {
            chance_male = gender_val_map[gender_val];
            chance_female = 1.0 - chance_male;
        }
    }

    string base_pkmn_gen2impl::get_info()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1.c_str() % type2.c_str());
        
        return str(boost::format(
            "%s (#%d)\n"
            "Type: %s\n"
            "Stats: %d, %d, %d, %d, %d, %d"
            ) % display_name.c_str() % nat_pokedex_num
              % types_str.c_str()
              % baseHP % baseATK % baseDEF % baseSATK % baseSDEF % baseSPD
        );
    }

    string base_pkmn_gen2impl::get_info_verbose()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1 % type2);

        return str(boost::format(
            "%s (#%d)\n"
            "%s Pokémon\n"
            "Type: %s\n"
            "%d m, %d kg\n"
            "Base Stats:\n"
            " - HP: %d\n"
            " - Attack: %d\n"
            " - Defense: %d\n"
            " - Special Attack: %d\n"
            " - Special Defense: %d\n"
            " - Speed: %d\n"
            " - Experience Yield: %d\n"
            ) % display_name.c_str() % nat_pokedex_num
              % species.c_str()
              % types_str.c_str()
              % height % weight
              % baseHP
              % baseATK
              % baseDEF
              % baseSATK
              % baseSDEF
              % baseSPD
              % exp_yield
        );
    }

    dict<string,int> base_pkmn_gen2impl::get_base_stats()
    {
        dict<string,int> stats;
        stats["HP"] = baseHP;
        stats["ATK"] = baseATK;
        stats["DEF"] = baseDEF;
        stats["SATK"] = baseSATK;
        stats["SDEF"] = baseSDEF;
        stats["SPD"] = baseSPD;
        return stats;
    }

    dict<string,int> base_pkmn_gen2impl::get_ev_yields()
    {
        dict<string,int> stats;
        stats["HP"] = baseHP;
        stats["ATK"] = baseATK;
        stats["DEF"] = baseDEF;
        stats["SPCL"] = baseSATK;
        stats["SPD"] = baseSPD;
        return stats;
    }

}
