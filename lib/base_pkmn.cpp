/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>

#include "base_pkmn_gen1impl.hpp"
#include "base_pkmn_gen2impl.hpp"
#include "base_pkmn_gen3impl.hpp"
#include "internal.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
	base_pkmn::base_pkmn(string identifier, int gen, SQLite::Database *db, bool query_moves)
	{
	    from_gen = gen;
        database_identifier = identifier;
    
        string query_string;

        //Fail if Pokémon's generation_id > specified gen
        query_string = "SELECT * FROM pokemon_species WHERE identifier='" + identifier + "'";
        SQLite::Statement pokemon_species_query(*db, query_string.c_str());
        pokemon_species_query.executeStep();
        int gen_id = pokemon_species_query.getColumn(2); //generation_id
        if(gen_id > gen)
        {
            string error_message = identifier + " not present in Generation " + to_string(gen) + ".";
            throw runtime_error(error_message.c_str());
        }

        //After Pokemon verified as valid, generate next available queries
        species_id = pokemon_species_query.getColumn(0); //id
        query_string = "SELECT id FROM pokemon WHERE species_id=" + to_string(species_id);
        pkmn_id = db->execAndGet(query_string.c_str());

        query_string = "SELECT * FROM pokemon WHERE id=" + to_string(pkmn_id);
        SQLite::Statement pokemon_query(*db, query_string.c_str());
        pokemon_query.executeStep();

        query_string = "SELECT * FROM pokemon_species_names WHERE pokemon_species_id=" + to_string(species_id);
        SQLite::Statement pokemon_species_names_query(*db, query_string.c_str());
        pokemon_species_names_query.executeStep();

        //Get available values from queries
        nat_pokedex_num = species_id;
        display_name = pokemon_species_names_query.getColumnStr(2); //name
        species = pokemon_species_names_query.getColumnStr(3); //genus
        height = pokemon_query.getColumn(2); //height
        height /= 10;
        weight = pokemon_query.getColumn(3); //weight
        weight /= 10;
        exp_yield = pokemon_query.getColumn(4); //base_experience

        //Type 1
        query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_id) + " AND slot=1";
        int type1_id = db->execAndGet(query_string.c_str(), identifier);
        query_string = "SELECT name FROM type_names WHERE type_id=" + to_string(type1_id);
        type1 = db->execAndGetStr(query_string.c_str(), identifier);
        
        //Type 2 (may be empty)
        query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_id) + " AND slot=2";
        int type2_id;
        SQLite::Statement pokemon_types_query(*db, query_string.c_str());
        if(pokemon_types_query.executeStep()) //Will be false if no database entry exists
        {
            type2_id = pokemon_types_query.getColumn(0); //type_id
            query_string = "SELECT name FROM type_names WHERE type_id=" + to_string(type2_id);
            type2 = db->execAndGetStr(query_string.c_str(), identifier);
        }
        else type2 = "None";
       
        //Stats
        query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id) + " AND stat_id IN (1,2,3,6)";
        SQLite::Statement pokemon_stats_query(*db, query_string.c_str());
        pokemon_stats_query.executeStep();
        baseHP = pokemon_stats_query.getColumn(0); //base_stat
        pokemon_stats_query.executeStep();
        baseATK = pokemon_stats_query.getColumn(0); //base_stat
        pokemon_stats_query.executeStep();
        baseDEF = pokemon_stats_query.getColumn(0); //base_stat
        pokemon_stats_query.executeStep();
        baseSPD = pokemon_stats_query.getColumn(0); //base_stat

        //version_group_ids in database
        string version_group_ids[] = {"(1,2,15)", "(3,4,16)", "(5,6,7,12,13)", "(8,9,10)", "(11,14)"};

        //Get legal moves
		if(query_moves)
		{
            query_string = "SELECT move_id FROM pokemon_moves WHERE pokemon_id=" + to_string(pkmn_id) + " AND version_group_id IN"
                         + version_group_ids[gen-1];
            SQLite::Statement legal_move_query(*db, query_string.c_str());
            while(legal_move_query.executeStep())
            {
                int move_id = legal_move_query.getColumn(0); //move_id
   
                query_string = "SELECT identifier FROM moves WHERE id=" + to_string(move_id); 
                string move_identifier = db->execAndGetStr(query_string.c_str(), "");
                legal_moves.push_back(base_move::make(move_identifier, gen));
            }
		}
        else legal_moves.clear();
	}
	
    base_pkmn::sptr base_pkmn::make(string identifier, int gen, bool query_moves)
    {
        try
        {
            //Match database's identifier format
            to_database_format(&identifier);

            if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

            SQLite::Database db(get_database_path().c_str());
            string query_string;

            switch(gen)
            {
                case 1:
                    return sptr(new base_pkmn_gen1impl(identifier, &db, query_moves));

                case 2:
                    return sptr(new base_pkmn_gen2impl(identifier, &db, query_moves));

                default:
                    return sptr(new base_pkmn_gen3impl(identifier, gen, &db, query_moves));
            }
        }
        catch(const exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    string base_pkmn::get_display_name(void) {return display_name;}

    int base_pkmn::get_nat_pokedex_num(void) {return nat_pokedex_num;}

    dict<int, std::string> base_pkmn::get_types(void)
    {
        dict<int, std::string> type_dict;
        type_dict[0] = type1;
        type_dict[1] = type2;
        return type_dict;
    }

    double base_pkmn::get_height(void) {return height;}

    double base_pkmn::get_weight(void) {return weight;}

	void base_pkmn::get_evolutions(vector<sptr>& evolution_vec)
	{
        evolution_vec.clear();

		SQLite::Database db(get_database_path().c_str());
		string query_string;
		
        vector<int> evolution_ids;
        vector<int> to_erase;
        query_string = "SELECT id FROM pokemon_species WHERE evolves_from_species_id=" + to_string(species_id);
        SQLite::Statement query(db, query_string.c_str());
        while(query.executeStep())
        {
            int evol_id = query.getColumn(0); //id
            evolution_ids.push_back(evol_id);
        }

        //Evolutions may not be present in specified generation
        for(unsigned int i = 0; i < evolution_ids.size(); i++)
        {
            query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(evolution_ids[i]);
            int generation_id = db.execAndGet(query_string.c_str(), database_identifier);
            if(generation_id > from_gen) to_erase.push_back(i);
        }
        for(int j = to_erase.size()-1; j >= 0; j--)
        {
            evolution_ids.erase(evolution_ids.begin() + to_erase[j]);
        }

        //Fill vector with sptrs of all evolutions
        for(unsigned int i = 0; i < evolution_ids.size(); i++)
        {
            //Get identifier for Pokémon
            query_string = "SELECT identifier FROM pokemon_species WHERE id=" + to_string(evolution_ids[i]);
            string evol_identifier = db.execAndGetStr(query_string.c_str(), "No string");

            evolution_vec.push_back(make(evol_identifier, from_gen, false));
        }
	}
	
    bool base_pkmn::is_fully_evolved()
    {
        vector<sptr> evolution_vec;
        get_evolutions(evolution_vec);

        return (evolution_vec.begin() == evolution_vec.end());
    }

    vector<base_move::sptr> base_pkmn::get_legal_moves(void) {return legal_moves;}

    void get_pkmn_of_type(vector<base_pkmn::sptr> &pkmn_vector, string type1, string type2, int gen, bool lax)
    {
        pkmn_vector.clear();

        SQLite::Database db(get_database_path().c_str());
        string query_string;
        vector<string> names;
        int pkmn_id, type1_id, type2_id;

        //Get type IDs
        query_string = "SELECT type_id FROM type_names WHERE name='" + type1 + "'";
        type1_id = db.execAndGet(query_string.c_str(), type1);
        if(type2 != "None" and type2 != "Any")
        {
            query_string = "SELECT type_id FROM type_names WHERE name='" + type2 + "'";
            type2_id = db.execAndGet(query_string.c_str(), type2);
        }

        if((type2 == "None" or type2 == "Any") and lax)
        {
            //Get IDs of Pokémon
            query_string = "SELECT pokemon_id FROM pokemon_types WHERE type_id=" + to_string(type1_id);
            SQLite::Statement pokemon_types_query(db, query_string.c_str());

            //Get any Pokémon of specified type (by itself or paired with any other)
            while(pokemon_types_query.executeStep())
            {
                pkmn_id = pokemon_types_query.getColumn(0); //pokemon_id

                query_string = "SELECT species_id FROM pokemon WHERE id=" + to_string(pkmn_id);
                int species_id = db.execAndGet(query_string.c_str());

                query_string = "SELECT identifier FROM pokemon_species WHERE id=" + to_string(species_id);
                string pkmn_name = db.execAndGetStr(query_string.c_str(), "No string");

                //Get generation ID to restrict list
                query_string = "SELECT generation_id FROM pokemon_species WHERE identifier='" + pkmn_name + "'";
                int generation_id = db.execAndGet(query_string.c_str(), pkmn_name);
                if(generation_id <= gen) names.push_back(pkmn_name);
            }
        }
        else
        {
            //Get IDs of Pokémon matching first type
            vector<int> pkmn_ids;
            query_string = "SELECT pokemon_id FROM pokemon_types WHERE type_id=" + to_string(type1_id);
            SQLite::Statement pokemon_types_id_query(db, query_string.c_str());

            while(pokemon_types_id_query.executeStep()) pkmn_ids.push_back(pokemon_types_id_query.getColumn(0));

            vector<int> to_erase;
            if(type2 == "None")
            {
                //If only one type is specified, find number of entries with that ID and remove duplicates
                for(unsigned int i = 0; i < pkmn_ids.size(); i++)
                {
                    int pkmn_count = 0; //Number of types Pokémon appears in pokemon_moves
                    query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_ids[i]);
                    SQLite::Statement inner_query(db, query_string.c_str());
                    while(inner_query.executeStep()) pkmn_count++;

                    if(pkmn_count > 1) to_erase.push_back(i);
                }
            }
            else
            {
                //See if entry exists for other type, add to to_erase if not
                for(unsigned int i = 0; i < pkmn_ids.size(); i++)
                {
                    query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_ids[i])
                                 + " AND type_id=" + to_string(type2_id);
                    SQLite::Statement inner_query(db, query_string.c_str());
                    if(not inner_query.executeStep()) to_erase.push_back(i);
                }
            }

            //Erase invalid entries
            for(unsigned int i = to_erase.size()-1; i >= 0; i--) pkmn_ids.erase(pkmn_ids.begin() + to_erase[i]);

            //Get identifiers for remaining entries
            for(unsigned int i = 0; i < pkmn_ids.size(); i++)
            {
                query_string = "SELECT species_id FROM pokemon WHERE id=" + to_string(pkmn_ids[i]);
                int species_id = db.execAndGet(query_string.c_str());

                query_string = "SELECT identifier FROM pokemon_species WHERE id=" + to_string(species_id);
                string pkmn_name = db.execAndGetStr(query_string.c_str(), "No string");

                //Get generation ID to restrict list
                query_string = "SELECT generation_id FROM pokemon_species WHERE identifier='" + pkmn_name + "'";
                int generation_id = db.execAndGet(query_string.c_str(), pkmn_name);
                if(generation_id <= gen) names.push_back(pkmn_name);
            }
        }

        for(unsigned int i = 0; i < names.size(); i++) pkmn_vector.push_back(base_pkmn::make(names[i], gen, false));
    }
}
