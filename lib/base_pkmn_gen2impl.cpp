/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "base_pkmn_gen2impl.hpp"
#include <boost/format.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <stdio.h>

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen2impl::base_pkmn_gen2impl(string identifier)
    {
        from_gen = 2;
        database_identifier = identifier;
    
        SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
        string query_string;

        //Fail if Pokémon's generation_id > 1
        query_string = str(boost::format("SELECT generation_id FROM pokemon_species WHERE identifier='%s'")
                                         % identifier.c_str());
        int gen_id = db.execAndGet(query_string.c_str(), identifier);
        if(gen_id > 2)
        {
            string error_message = str(boost::format("%s not present in Generation 2.")
                                                     % identifier.c_str());
            throw runtime_error(error_message.c_str());
        }

        //Get relevant database IDs
        query_string = str(boost::format("SELECT id FROM pokemon_species WHERE identifier='%s'")
                                         % identifier.c_str());
        species_id = db.execAndGet(query_string.c_str(), identifier);
        query_string = str(boost::format("SELECT id FROM pokemon WHERE species_id=%d")
                                         % species_id);
        pkmn_id = db.execAndGet(query_string.c_str(), identifier);

        //National Pokedex number same as species ID
        nat_pokedex_num = species_id;

        //Display name and species are in same table
        query_string = str(boost::format("SELECT name FROM pokemon_species_names WHERE pokemon_species_id=%d AND local_language_id=9")
                                         % species_id);
        display_name = db.execAndGetStr(query_string.c_str(), identifier);
        query_string = str(boost::format("SELECT genus FROM pokemon_species_names WHERE pokemon_species_id=%d AND local_language_id=9")
                                         % species_id);
        species = db.execAndGetStr(query_string.c_str(), identifier);

        //Height and weight are stored as integers and must be divided by 10
        query_string = str(boost::format("SELECT height FROM pokemon WHERE id=%d")
                                         % pkmn_id);
        height = db.execAndGet(query_string.c_str(), identifier);
        height /= 10;
        query_string = str(boost::format("SELECT weight FROM pokemon WHERE id=%d")
                                         % pkmn_id);
        weight = db.execAndGet(query_string.c_str(), identifier);
        weight /= 10;

        //Type 1
        query_string = str(boost::format("SELECT type_id FROM pokemon_types WHERE pokemon_id=%d AND slot=1")
                                         % pkmn_id);
        int type1_id = db.execAndGet(query_string.c_str(), identifier);
        query_string = str(boost::format("SELECT name FROM type_names WHERE type_id=%d AND local_language_id=9")
                                         % type1_id);
        type1 = db.execAndGetStr(query_string.c_str(), identifier);
        
        //Type 2 (may be empty)
        query_string = str(boost::format("SELECT type_id FROM pokemon_types WHERE pokemon_id=%d AND slot=2")
                                         % pkmn_id);
        int type2_id;
        SQLite::Statement query(db, query_string.c_str());
        if(query.executeStep()) //Will be false if no database entry exists
        {
            type2_id = query.getColumn(0);
            query_string = str(boost::format("SELECT name FROM type_names WHERE type_id=%d AND local_language_id=9")
                                             % type2_id);
            type2 = db.execAndGetStr(query_string.c_str(), identifier);
        }
        else type2 = "None";
       
        //Stats
        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=1")
                                         % pkmn_id);
        baseHP = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=2")
                                         % pkmn_id);
        baseATK = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=3")
                                         % pkmn_id);
        baseDEF = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=4")
                                         % pkmn_id);
        baseSATK = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=5")
                                         % pkmn_id);
        baseSDEF = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=6")
                                         % pkmn_id);
        baseSPD = db.execAndGet(query_string.c_str(), identifier); 

        //Effort yields
        query_string = str(boost::format("SELECT effort FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=1")
                                         % pkmn_id);
        evHP = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT effort FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=2")
                                         % pkmn_id);
        evATK = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT effort FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=3")
                                         % pkmn_id);
        evDEF = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT effort FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=4")
                                         % pkmn_id);
        evSATK = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT effort FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=5")
                                         % pkmn_id);
        evSDEF = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT effort FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=6")
                                         % pkmn_id);
        evSPD = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_experience FROM pokemon WHERE id='%s'")
                                         % pkmn_id);
        exp_yield = db.execAndGet(query_string.c_str(), identifier);

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

    map<string,int> base_pkmn_gen2impl::get_base_stats()
    {
        map<string,int> stats;
        stats["HP"] = baseHP;
        stats["ATK"] = baseATK;
        stats["DEF"] = baseDEF;
        stats["SATK"] = baseSATK;
        stats["SDEF"] = baseSDEF;
        stats["SPD"] = baseSPD;
        return stats;
    }

    map<string,int> base_pkmn_gen2impl::get_ev_yields()
    {
        map<string,int> stats;
        stats["HP"] = evHP;
        stats["ATK"] = evATK;
        stats["DEF"] = evDEF;
        stats["SATK"] = evSATK;
        stats["SDEF"] = evSDEF;
        stats["SPD"] = evSPD;
        return stats;
    }

}
