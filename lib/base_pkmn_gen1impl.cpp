/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <stdio.h>

#include <boost/format.hpp>

#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "base_pkmn_gen1impl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen1impl::base_pkmn_gen1impl(string identifier):
                                           base_pkmn(identifier, 1) {}

    string base_pkmn_gen1impl::get_info()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<string, int> stats = get_base_stats();

        string stats_str = to_string(stats["HP"]) + ", " + to_string(stats["ATK"]) + ", "
                         + to_string(stats["DEF"]) + ", " + to_string(stats["SPD"]) + ", "
                         + to_string(stats["SPCL"]);

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      + "Type: " + types_str + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string base_pkmn_gen1impl::get_info_verbose()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<string, int> stats = get_base_stats();

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      //+ species + " Pokémon\n"
                      + "Type: " + types_str + "\n"
                      + to_string(get_height()) + " m, " + to_string(get_weight()) + " kg\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(stats["HP"]) + "\n"
                      + " - Attack: " + to_string(stats["ATK"]) + "\n"
                      + " - Defense: " + to_string(stats["DEF"]) + "\n"
                      + " - Speed: " + to_string(stats["SPD"]) + "\n"
                      + " - Special: " + to_string(stats["SPCL"]);

        return output_string;
    }

    dict<string,int> base_pkmn_gen1impl::get_base_stats()
    {
        dict<string,int> stats;

        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id) +
                       " AND stat_id IN (1,2,3,6,9)";
        SQLite::Statement stats_query(db, query_string.c_str());

        stats_query.executeStep();
        stats["HP"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["ATK"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["DEF"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SPD"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SPCL"] = stats_query.getColumn(0);
        return stats;
    }

    dict<string,int> base_pkmn_gen1impl::get_ev_yields()
    {
        //In Generation 1, EV yields were the same as the corresponding base stat
        return get_base_stats();
    }

    bool base_pkmn_gen1impl::has_gender_differences(void) {return false;}

    string base_pkmn_gen1impl::get_sprite_path(bool is_male, bool is_shiny)
    {
        //Gender and shininess don't matter in Gen 1
        return male_sprite_path;
    }
}
