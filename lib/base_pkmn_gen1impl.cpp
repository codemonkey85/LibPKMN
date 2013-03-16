/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "base_pkmn_gen1impl.hpp"
#include <boost/format.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <stdio.h>

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen1impl::base_pkmn_gen1impl(string identifier)
    {
        from_gen = 1;
        database_identifier = identifier;
    
        SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
        string query_string;

        //Fail if Pokémon's generation_id > 1
        query_string = str(boost::format("SELECT generation_id FROM pokemon_species WHERE identifier='%s'")
                                         % identifier.c_str());
        int gen_id = db.execAndGet(query_string.c_str(), identifier);
        if(gen_id > 1)
        {
            string error_message = str(boost::format("%s not present in Generation 1.")
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

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=6")
                                         % pkmn_id);
        baseSPD = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=%d AND stat_id=9")
                                         % pkmn_id);
        baseSPCL = db.execAndGet(query_string.c_str(), identifier); 

        query_string = str(boost::format("SELECT base_experience FROM pokemon WHERE id='%s'")
                                         % pkmn_id);
        exp_yield = db.execAndGet(query_string.c_str(), identifier);
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
