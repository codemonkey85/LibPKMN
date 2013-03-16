/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <boost/format.hpp>
#include <iostream>
#include <string>
#include "base_pkmn_gen1impl.hpp"
#include "base_pkmn_gen2impl.hpp"
#include "base_pkmn_gen3impl.hpp"
#include <pkmnsim/base_pkmn.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <vector>

using namespace std;

namespace pkmnsim
{
	base_pkmn::base_pkmn(string identifier, int gen)
	{
	    from_gen = gen;
        database_identifier = identifier;
    
        SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
        string query_string;

        //Fail if Pokémon's generation_id > 1
        query_string = str(boost::format("SELECT generation_id FROM pokemon_species WHERE identifier='%s'")
                                         % identifier.c_str());
        int gen_id = db.execAndGet(query_string.c_str(), identifier);
        if(gen_id > gen)
        {
            string error_message = str(boost::format("%s not present in Generation %d.")
                                                     % identifier.c_str() % gen);
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
        query_string = str(boost::format("SELECT base_experience FROM pokemon WHERE id='%s'")
                                         % pkmn_id);
        exp_yield = db.execAndGet(query_string.c_str(), identifier);
	}

    base_pkmn::sptr base_pkmn::make(string identifier, int gen)
    {
        transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);

        if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

        switch(gen)
        {
            case 1:
                return sptr(new base_pkmn_gen1impl(identifier));

            case 2:
                return sptr(new base_pkmn_gen2impl(identifier));

            default:
                return sptr(new base_pkmn_gen3impl(identifier, gen));
        }
    }
	
	void base_pkmn::get_evolutions(vector<sptr>& evolution_vec)
	{
        evolution_vec.clear();

		SQLite::Database db("@PKMNSIM_DB@");
		string query_string;
		
        vector<int> evolution_ids;
        vector<int> to_erase;
        query_string = str(boost::format("SELECT id FROM pokemon_species WHERE evolves_from_species_id=%d")
                                         % species_id);
        SQLite::Statement query(db, query_string.c_str());
        while(query.executeStep())
        {
            int evol_id = query.getColumn(0);
            evolution_ids.push_back(evol_id);
        }

        //Evolutions may not be present in specified generation
        for(int i = 0; i < evolution_ids.size(); i++)
        {
            query_string = str(boost::format("SELECT generation_id FROM pokemon_species WHERE id=%d")
                                             % evolution_ids[i]);
            int generation_id = db.execAndGet(query_string.c_str(), database_identifier);
            if(generation_id > from_gen) to_erase.push_back(i);
        }
        for(int j = to_erase.size()-1; j >= 0; j--)
        {
            evolution_ids.erase(evolution_ids.begin() + to_erase[j]);
        }

        //Fill vector with sptrs of all evolutions
        for(int i = 0; i < evolution_ids.size(); i++)
        {
            //Get identifier for Pokémon
            query_string = str(boost::format("SELECT identifier FROM pokemon_species WHERE id=%d")
                                             % evolution_ids[i]);
            string evol_identifier = db.execAndGetStr(query_string.c_str(), "No string");

            evolution_vec.push_back(make(evol_identifier, from_gen));
        }
	}
	
    bool base_pkmn::is_fully_evolved()
    {
        vector<sptr> evolution_vec;
        get_evolutions(evolution_vec);

        return (evolution_vec.begin() == evolution_vec.end());
    }

    base_pkmn::sptr get_base_pkmn(string identifier, int gen) //More user-friendly
    {
        return base_pkmn::make(identifier, gen);
    }

    void get_pkmn_of_type(vector<base_pkmn::sptr> &pkmn_vector, string type1, string type2, int gen, bool lax)
    {
        pkmn_vector.clear();

        SQLite::Database db("@PKMNSIM_DB@"); //Filepath to be filled by CMake
        string query_string;
        vector<string> names;
        int pkmn_id, type1_id, type2_id;

        //Get type IDs
        query_string = str(boost::format("SELECT type_id FROM type_names WHERE name='%s'")
                                         % type1.c_str());
        type1_id = db.execAndGet(query_string.c_str(), type1);
        if(type2 != "None")
        {
            query_string = str(boost::format("SELECT type_id FROM type_names WHERE name='%s'")
                                             % type2.c_str());
            type2_id = db.execAndGet(query_string.c_str(), type2);
        }

        if(type2 == "None" and lax)
        {
            //Get IDs of Pokémon
            query_string = str(boost::format("SELECT pokemon_id FROM pokemon_types WHERE type_id=%d")
                                             % type1_id);
            SQLite::Statement query(db, query_string.c_str());

            //Get any Pokémon of specified type (by itself or paired with any other)
            while(query.executeStep())
            {
                pkmn_id = query.getColumn(0);

                query_string = str(boost::format("SELECT species_id FROM pokemon WHERE id=%d")
                                                 % pkmn_id);
                int species_id = db.execAndGet(query_string.c_str());

                query_string = str(boost::format("SELECT identifier FROM pokemon_species WHERE id=%d")
                                                 % species_id);
                string pkmn_name = db.execAndGetStr(query_string.c_str(), "No string");

                //Get generation ID to restrict list
                query_string = str(boost::format("SELECT generation_id FROM pokemon_species WHERE identifier='%s'")
                                                 % pkmn_name);
                int generation_id = db.execAndGet(query_string.c_str(), pkmn_name);
                if(generation_id <= gen) names.push_back(pkmn_name);
            }
        }
        else
        {
            //Get IDs of Pokémon matching first type
            vector<int> pkmn_ids;
            query_string = str(boost::format("SELECT pokemon_id FROM pokemon_types WHERE type_id=%d")
                                             % type1_id);
            SQLite::Statement query(db, query_string.c_str());

            while(query.executeStep()) pkmn_ids.push_back(query.getColumn(0));

            vector<int> to_erase;
            if(type2 == "None")
            {
                //If only one type is specified, find number of entries with that ID and remove duplicates
                for(int i = 0; i < pkmn_ids.size(); i++)
                {
                    int pkmn_count = 0; //Number of types Pokémon appears in pokemon_moves
                    query_string = str(boost::format("SELECT type_id FROM pokemon_types WHERE pokemon_id=%d")
                                                     % pkmn_ids[i]);
                    SQLite::Statement inner_query(db, query_string.c_str());
                    while(inner_query.executeStep()) pkmn_count++;

                    if(pkmn_count > 1) to_erase.push_back(i);
                }
            }
            else
            {
                //See if entry exists for other type, add to to_erase if not
                for(int i = 0; i < pkmn_ids.size(); i++)
                {
                    query_string = str(boost::format("SELECT type_id FROM pokemon_types WHERE pokemon_id=%d AND type_id=%d")
                                                     % pkmn_ids[i] % type2_id);
                    SQLite::Statement inner_query(db, query_string.c_str());
                    if(not inner_query.executeStep()) to_erase.push_back(i);
                }
            }

            //Erase invalid entries
            for(int i = to_erase.size()-1; i >= 0; i--) pkmn_ids.erase(pkmn_ids.begin() + to_erase[i]);

            //Get identifiers for remaining entries
            for(int i = 0; i < pkmn_ids.size(); i++)
            {
                query_string = str(boost::format("SELECT species_id FROM pokemon WHERE id=%d")
                                                 % pkmn_ids[i]);
                int species_id = db.execAndGet(query_string.c_str());

                query_string = str(boost::format("SELECT identifier FROM pokemon_species WHERE id=%d")
                                                 % species_id);
                string pkmn_name = db.execAndGetStr(query_string.c_str(), "No string");

                //Get generation ID to restrict list
                query_string = str(boost::format("SELECT generation_id FROM pokemon_species WHERE identifier='%s'")
                                                 % pkmn_name);
                int generation_id = db.execAndGet(query_string.c_str(), pkmn_name);
                if(generation_id <= gen) names.push_back(pkmn_name);
            }
        }

        for(int i = 0; i < names.size(); i++) pkmn_vector.push_back(base_pkmn::make(names[i], gen));
    }
}
