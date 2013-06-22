/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <stdio.h>

#include "base_pkmn_gen1impl.hpp"
#include "internal.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen1impl::base_pkmn_gen1impl(string identifier, bool query_moves):
                                           base_pkmn(identifier, 1, query_moves)
    {
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id)
                            + " AND stat_id=9";
        baseSPCL = db.execAndGet(query_string.c_str(), identifier); 
    }

    string base_pkmn_gen1impl::get_info()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = type1 + "/" + type2;

        string stats_str = to_string(baseHP) + ", " + to_string(baseATK) + ", "
                         + to_string(baseDEF) + ", " + to_string(baseSPD) + ", "
                         + to_string(baseSPCL);

        string output_string;
        output_string = display_name + " (#" + to_string(nat_pokedex_num) + ")\n"
                      + "Type: " + types_str + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string base_pkmn_gen1impl::get_info_verbose()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = type1 + "/" + type2;

        string output_string;
        output_string = display_name + " (#" + to_string(nat_pokedex_num) + ")\n"
                      + species + " Pokémon\n"
                      + to_string(height) + " m, " + to_string(weight) + " kg\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(baseHP) + "\n"
                      + " - Attack: " + to_string(baseATK) + "\n"
                      + " - Defense: " + to_string(baseDEF) + "\n"
                      + " - Speed: " + to_string(baseSPD) + "\n"
                      + " - Special: " + to_string(baseSPCL) + "\n"
                      + " - Experience Yield: " + to_string(exp_yield);

        return output_string;
    }

    dict<string,int> base_pkmn_gen1impl::get_base_stats()
    {
        dict<string,int> stats;
        stats["HP"] = baseHP;
        stats["ATK"] = baseATK;
        stats["DEF"] = baseDEF;
        stats["SPD"] = baseSPD;
        stats["SPCL"] = baseSPCL;
        return stats;
    }

    dict<string,int> base_pkmn_gen1impl::get_ev_yields()
    {
        //In Generation 1, EV yields were the same as the corresponding base stat
        return get_base_stats();
    }
}
