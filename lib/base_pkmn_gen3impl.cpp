/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "base_pkmn_gen3impl.hpp"
#include <boost/format.hpp>
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen3impl::base_pkmn_gen3impl(string identifier, int gen, SQLite::Database *db,
                                           bool query_moves): base_pkmn(identifier, gen, db, query_moves)
    {    
        string query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=4")
                                         % pkmn_id);
        baseSATK = db->execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=5")
                                         % pkmn_id);
        baseSDEF = db->execAndGet(query_string.c_str(), identifier); 

        //Effort yields
        query_string = str(boost::format("SELECT effort FROM pokemon_stats WHERE pokemon_id=%d AND stat_id IN (1,2,3,4,5,6)")
                                         % pkmn_id);
        SQLite::Statement pokemon_stats_query(*db, query_string.c_str());
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

        //Ability 1 (guaranteed)
        query_string = str(boost::format("SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=%d AND slot=1")
                                         % pkmn_id);
        int ability1_id = db->execAndGet(query_string.c_str(), identifier);
        query_string = str(boost::format("SELECT name FROM ability_names WHERE ability_id=%d")
                                         % ability1_id);
        ability1 = db->execAndGetStr(query_string.c_str(), identifier);

        //Ability 2 (not guaranteed, and if exists, might not exist in specified generation
        query_string = str(boost::format("SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=%d AND slot=2")
                                         % pkmn_id);
        SQLite::Statement ability2_query(*db, query_string.c_str());
        if(ability2_query.executeStep()) //Will be false if no entry exists
        {
            int ability2_id = ability2_query.getColumn(0); //ability_id
            query_string = str(boost::format("SELECT generation_id FROM abilities WHERE id=%d")
                                             % ability2_id);
            int generation_id = db->execAndGet(query_string.c_str(), identifier);
            if(generation_id > gen) ability2 = "None";
            else
            {
                query_string = str(boost::format("SELECT name FROM ability_names WHERE ability_id=%d")
                                                 % ability2_id);
                ability2 = db->execAndGetStr(query_string.c_str(), identifier);
            }
        }
        else ability2 = "None";

        //Ability 3 (hidden ability, only in Gen 5, even then not guaranteed)
        if(gen == 5)
        {
            query_string = str(boost::format("SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=%d AND slot=3")
                                             % pkmn_id);
            SQLite::Statement ability3_query(*db, query_string.c_str());
            if(ability3_query.executeStep()) //Will be false if no entry exists
            {
                int ability3_id = db->execAndGet(query_string.c_str(), identifier);
                query_string = str(boost::format("SELECT name FROM ability_names WHERE ability_id=%d")
                                                 % ability3_id);
                ability3 = db->execAndGetStr(query_string.c_str(), identifier);
            }
            else ability3 = "None";
        }
        else ability3 = "None";

    }

    string base_pkmn_gen3impl::get_info()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1 % type2);

        return str(boost::format(
            "%s (#%d)\n"
            "Type: %s\n"
            "Stats: %d, %d, %d, %d, %d, %d"
            ) % display_name.c_str() % nat_pokedex_num
              % types_str.c_str()
              % baseHP % baseATK % baseDEF % baseSATK % baseSDEF % baseSPD
        );
    }

    string base_pkmn_gen3impl::get_info_verbose()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1 % type2);

        string abilities_str;
        if(ability2 == "None") abilities_str = ability1; //1 ability
        else if(ability3 == "None") abilities_str = str(boost::format("%s, %s")
                                                        % ability1 % ability2); //2 abilities
        else abilities_str = str(boost::format("%s, %s, %s") 
                                 % ability1 % ability2 % ability3);

        return str(boost::format(
            "%s (#%d)\n"
            "%s Pokémon\n"
            "Type: %s\n"
            "%d m, %d kg\n"
            "Ability: %s\n"
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
              % abilities_str.c_str()
              % baseHP
              % baseATK
              % baseDEF
              % baseSATK
              % baseSDEF
              % baseSPD
              % exp_yield
        );
    }


    dict<string, int> base_pkmn_gen3impl::get_base_stats()
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

    dict<string, int> base_pkmn_gen3impl::get_ev_yields()
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

    dict<int, string> base_pkmn_gen3impl::get_abilities()
    {
        dict<int, string> abilities;
        abilities[0] = ability1;
        abilities[1] = ability2;
        abilities[2] = ability3;
        return abilities;
    }
}
