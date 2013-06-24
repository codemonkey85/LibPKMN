/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>
#include <stdio.h>

#include "base_pkmn_gen2impl.hpp"
#include "internal.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen2impl::base_pkmn_gen2impl(string identifier, bool query_moves):
                                           base_pkmn(identifier, 2, query_moves)
    {
        SQLite::Database db(get_database_path().c_str());

        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id)
                            + " AND stat_id=4";
        baseSATK = db.execAndGet(query_string.c_str(), identifier); 

        query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id)
                     + " AND stat_id=5";
        baseSDEF = db.execAndGet(query_string.c_str(), identifier); 

        //Gender rates
        map<int, double> gender_val_map; //Double is percentage male
        gender_val_map[0] = 1.0;
        gender_val_map[1] = 0.875;
        gender_val_map[2] = 0.75;
        gender_val_map[4] = 0.5;
        gender_val_map[6] = 0.25;
        gender_val_map[8] = 0.0;

        query_string = "SELECT gender_rate FROM pokemon_species WHERE id=" + to_string(species_id);
        int gender_val = db.execAndGet(query_string.c_str(), identifier);

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
        else types_str = type1 + "/" + type2;

        string stats_str = to_string(baseHP) + ", " + to_string(baseATK) + ", "
                         + to_string(baseDEF) + ", " + to_string(baseSATK) + ", "
                         + to_string(baseSDEF) + ", " + to_string(baseSPD);

        string output_string;
        output_string = display_name + " (#" + to_string(nat_pokedex_num) + ")\n"
                      + "Type: " + types_str + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string base_pkmn_gen2impl::get_info_verbose()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = type1 + "/" + type2;

        string output_string;
        output_string = display_name + " (#" + to_string(nat_pokedex_num) + ")\n"
                      + species + " Pokémon\n"
                      + "Type: " + types_str + "\n"
                      + to_string(height) + " m, " + to_string(weight) + " kg\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(baseHP) + "\n"
                      + " - Attack: " + to_string(baseATK) + "\n"
                      + " - Defense: " + to_string(baseDEF) + "\n"
                      + " - Special Attack: " + to_string(baseSATK) + "\n"
                      + " - Special Defense: " + to_string(baseSDEF) + "\n"
                      + " - Speed: " + to_string(baseSPD) + "\n"
                      + " - Experience Yield: " + to_string(exp_yield);
    
        return output_string;
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

    double base_pkmn_gen2impl::get_chance_male() {return chance_male;}

    double base_pkmn_gen2impl::get_chance_female() {return chance_female;}

    bool base_pkmn_gen2impl::has_gender_differences(void) {return false;}

    string base_pkmn_gen2impl::get_sprite_path(bool is_male, bool is_shiny)
    {
        if(is_male)
        {
            if(is_shiny) return male_shiny_sprite_path;
            else return male_sprite_path;
        }
        else
        {
            if(is_shiny) return female_shiny_sprite_path;
            else return female_sprite_path;
        }
    }
}
