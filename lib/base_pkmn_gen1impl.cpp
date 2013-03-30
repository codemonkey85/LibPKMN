/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "base_pkmn_gen1impl.hpp"
#include <boost/format.hpp>
#include "sqlitecpp/SQLiteCPP.h"
#include <stdio.h>

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen1impl::base_pkmn_gen1impl(string identifier, SQLite::Database *db): base_pkmn(identifier, 1, db)
    {
        string query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=9")
                                                % pkmn_id);
        baseSPCL = db->execAndGet(query_string.c_str(), identifier); 
    }

    string base_pkmn_gen1impl::get_info()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1 % type2);

        return str(boost::format(
            "%s (#%d)\n"
            "Type: %s\n"
            "Stats: %d, %d, %d, %d, %d"
            ) % display_name % nat_pokedex_num
              % types_str
              % baseHP % baseATK % baseDEF % baseSPD % baseSPCL
        );
    }

    string base_pkmn_gen1impl::get_info_verbose()
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
            " - Speed: %d\n"
            " - Special: %d\n"
            " - Experience Yield: %d\n"
            ) % display_name % nat_pokedex_num
              % species
              % types_str
              % height % weight
              % baseHP
              % baseATK
              % baseDEF
              % baseSPD
              % baseSPCL
              % exp_yield
        );
    }

    map<string,int> base_pkmn_gen1impl::get_base_stats()
    {
        map<string,int> stats;
        stats["HP"] = baseHP;
        stats["ATK"] = baseATK;
        stats["DEF"] = baseDEF;
        stats["SPD"] = baseSPD;
        stats["SPCL"] = baseSPCL;
        return stats;
    }

    map<string,int> base_pkmn_gen1impl::get_ev_yields()
    {
        //In Generation 1, EV yields were the same as the corresponding base stat
        return get_base_stats();
    }
}
