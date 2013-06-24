/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>

#include "base_pkmn_gen345impl.hpp"
#include "internal.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen345impl::base_pkmn_gen345impl(string identifier, int gen, bool query_moves):
                                           base_pkmn(identifier, gen, query_moves)
    {
        SQLite::Database db(get_database_path().c_str());

        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id)
                            + " AND stat_id=4";
        baseSATK = db.execAndGet(query_string.c_str(), identifier); 

        query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id)
                     + " AND stat_id=5";
        baseSDEF = db.execAndGet(query_string.c_str(), identifier); 

        //Effort yields
        query_string = "SELECT effort FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id)
                     + " AND stat_id IN (1,2,3,4,5,6)";
        SQLite::Statement pokemon_stats_query(db, query_string.c_str());
        pokemon_stats_query.executeStep();
        evHP = pokemon_stats_query.getColumn(0); //effort
        pokemon_stats_query.executeStep();
        evATK = pokemon_stats_query.getColumn(0); //effort
        pokemon_stats_query.executeStep();
        evDEF = pokemon_stats_query.getColumn(0); //effort
        pokemon_stats_query.executeStep();
        evSATK = pokemon_stats_query.getColumn(0); //effort
        pokemon_stats_query.executeStep();
        evSDEF = pokemon_stats_query.getColumn(0); //effort
        pokemon_stats_query.executeStep();
        evSPD = pokemon_stats_query.getColumn(0); //effort

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

        //Ability 1 (guaranteed)
        query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(pkmn_id)
                     + " AND slot=1";
        int ability1_id = db.execAndGet(query_string.c_str(), identifier);
        query_string = "SELECT name FROM ability_names WHERE ability_id=" + to_string(ability1_id);
        ability1 = db.execAndGetStr(query_string.c_str(), identifier);

        //Ability 2 (not guaranteed, and if exists, might not exist in specified generation
        query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(pkmn_id)
                     + " AND slot=2";
        SQLite::Statement ability2_query(db, query_string.c_str());
        if(ability2_query.executeStep()) //Will be false if no entry exists
        {
            int ability2_id = ability2_query.getColumn(0); //ability_id
            query_string = "SELECT generation_id FROM abilities WHERE id=" + to_string(ability2_id);
            int generation_id = db.execAndGet(query_string.c_str(), identifier);
            if(generation_id > gen) ability2 = "None";
            else
            {
                query_string = "SELECT name FROM ability_names WHERE ability_id=" + to_string(ability2_id);
                ability2 = db.execAndGetStr(query_string.c_str(), identifier);
            }
        }
        else ability2 = "None";

        //Ability 3 (hidden ability, only in Gen 5, even then not guaranteed)
        if(gen == 5)
        {
            query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(pkmn_id)
                         + " AND slot=3";
            SQLite::Statement ability3_query(db, query_string.c_str());
            if(ability3_query.executeStep()) //Will be false if no entry exists
            {
                int ability3_id = db.execAndGet(query_string.c_str(), identifier);
                query_string = "SELECT name FROM ability_names WHERE ability_id=" + to_string(ability3_id);
                ability3 = db.execAndGetStr(query_string.c_str(), identifier);
            }
            else ability3 = "None";
        }
        else ability3 = "None";

    }
    string base_pkmn_gen345impl::get_info()
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

    string base_pkmn_gen345impl::get_info_verbose()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = type1 + "/" + type2;

        string abilities_str;
        if(ability2 == "None") abilities_str = ability1; //1 ability
        else if(ability3 == "None") abilities_str = ability1 + ", " + ability2; //2 abilities
        else abilities_str = ability1 + ", " + ability2 + ", " + ability3;

        string output_string;
        output_string = display_name + " (#" + to_string(nat_pokedex_num) + ")\n"
                      + species + " Pokémon\n"
                      + "Type: " + types_str + "\n"
                      + to_string(height) + " m, " + to_string(weight) + " kg\n"
                      + "Abilities: " + abilities_str + "\n"
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

    dict<string, int> base_pkmn_gen345impl::get_base_stats()
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

    dict<string, int> base_pkmn_gen345impl::get_ev_yields()
    {
        dict<string, int> stats;
        stats["HP"] = evHP;
        stats["ATK"] = evATK;
        stats["DEF"] = evDEF;
        stats["SATK"] = evSATK;
        stats["SDEF"] = evSDEF;
        stats["SPD"] = evSPD;
        return stats;
    }

    double base_pkmn_gen345impl::get_chance_male() {return chance_male;}

    double base_pkmn_gen345impl::get_chance_female() {return chance_female;}

    dict<int, string> base_pkmn_gen345impl::get_abilities()
    {
        dict<int, string> abilities;
        abilities[0] = ability1;
        abilities[1] = ability2;
        abilities[2] = ability3;
        return abilities;
    }

    bool base_pkmn_gen345impl::has_gender_differences(void)
    {
        if(from_gen == 3) return false;
        else return has_gender_diff;
    }

    string base_pkmn_gen345impl::get_sprite_path(bool is_male, bool is_shiny)
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
