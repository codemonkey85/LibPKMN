/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "base_pokemon_impl.hpp"
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

#include "base_pokemon_impl.hpp"
#include "base_pokemon_gen1impl.hpp"
#include "base_pokemon_gen2impl.hpp"
#include "base_pokemon_modernimpl.hpp"

using namespace std;

namespace pkmnsim
{
    base_pokemon::sptr base_pokemon::make(unsigned int id, unsigned int game)
    {
        try
        {
            //Get generation from game enum
            int gen = database::get_generation(game);

            if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

            switch(gen)
            {
                case 1:
                    return sptr(new base_pokemon_gen1impl(id, game));

                case 2:
                    return sptr(new base_pokemon_gen2impl(id, game));

                default:
                    return sptr(new base_pokemon_modernimpl(id, game));
            }
        }
        catch(const exception &e)
        {
            cerr << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    base_pokemon_impl::base_pokemon_impl(unsigned int id, unsigned int game): base_pokemon()
    {
        from_game = game;
        from_gen = database::get_generation(from_game);

        switch(id)
        {
            case Species::NONE: //None
                pokemon_id = Species::NONE;
                species_id = Species::NONE;
                form_id = 0;
                pokemon_id = 0;
                species_id = 0;
                type1_id = 0;
                type2_id = 0;
                hp = 0;
                attack = 0;
                defense = 0;
                speed = 0;
                special = 0;
                special_attack = 0;
                special_defense = 0;
                break;

            case Species::INVALID: //Invalid
                pokemon_id = Species::INVALID;
                species_id = Species::INVALID;
                form_id = 0;
                pokemon_id = 0;
                species_id = 0;
                type1_id = 0;
                type2_id = 0;
                hp = 0;
                attack = 0;
                defense = 0;
                speed = 0;
                special = 0;
                special_attack = 0;
                special_defense = 0;
                break;

            default:
                from_game = game;
                species_id = id;
                
                SQLite::Database db(get_database_path().c_str());
                string query_string;
                
                //Get generation and name from specified game enum
                query_string = "SELECT name FROM version_names WHERE version_id=" + to_string(game);
                game_string = string((const char*)db.execAndGet(query_string.c_str()));
                
                //Fail if Pokemon's generation_id > specified gen
                query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(species_id);
                unsigned int gen_id = int(db.execAndGet(query_string.c_str()));
                if(gen_id > from_gen)
                {
                    string error_message = to_string(gen_id) + " > " + to_string(from_gen);
                    throw runtime_error(error_message.c_str());
                }

                //Get Pokemon ID from database
                query_string = "SELECT id FROM pokemon WHERE species_id=" + to_string(species_id);
                pokemon_id = int(db.execAndGet(query_string.c_str()));
                
                //Even though most attributes are queried from the database when called, stats take a long time when
                //doing a lot at once, so grab these upon instantiation
                query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pokemon_id) +
                               " AND stat_id IN (1,2,3,6)";
                SQLite::Statement stats_query(db, query_string.c_str());

                stats_query.executeStep();
                hp = int(stats_query.getColumn(0));
                stats_query.executeStep();
                attack = int(stats_query.getColumn(0));
                stats_query.executeStep();
                defense = int(stats_query.getColumn(0));
                stats_query.executeStep();
                speed = int(stats_query.getColumn(0));
                
                query_string = "SELECT type_id FROM pokemon_types WHERE slot=1 and pokemon_id=" + to_string(pokemon_id);
                type1_id = int(db.execAndGet(query_string.c_str()));
                
                //If there's only one type, this won't return anything
                query_string = "SELECT type_id FROM pokemon_types WHERE slot=2 and pokemon_id=" + to_string(pokemon_id);
                SQLite::Statement query(db, query_string.c_str());
                type2_id = (query.executeStep()) ? int(query.getColumn(0)) : Types::NONE;
        }
    }

    unsigned int base_pokemon_impl::get_pokedex_num() const {return species_id;}
    string base_pokemon_impl::get_pokedex_entry() const {return database::get_pokedex_entry(species_id, from_game);}

    dict<unsigned int, unsigned int> base_pokemon_impl::get_types() const
    {
        dict<unsigned int, unsigned int> types;
        types[0] = type1_id;
        types[1] = type2_id;
        return types;
    }
    
    double base_pokemon_impl::get_height() const
    {
        switch(species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT height FROM pokemon WHERE id=" + to_string(pokemon_id);
                return (double(db.execAndGet(query_string.c_str())) / 10.0);
        }
    }
    
    double base_pokemon_impl::get_weight() const
    {
        switch(species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT weight FROM pokemon WHERE id=" + to_string(pokemon_id);
                return (double(db.execAndGet(query_string.c_str())) / 10.0);
        }
    }
    
    void base_pokemon_impl::get_evolutions(b_pkmn_vec_t &evolution_vec) const
    {
        evolution_vec.clear();

        if(species_id != Species::NONE and species_id != Species::INVALID)
        {
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
                int generation_id = db.execAndGet(query_string.c_str());
                if(generation_id > from_gen) to_erase.push_back(i);
            }
            for(int j = to_erase.size()-1; j >= 0; j--)
            {
                evolution_ids.erase(evolution_ids.begin() + to_erase[j]);
            }

            //Fill vector with sptrs of all evolutions
            for(unsigned int i = 0; i < evolution_ids.size(); i++) evolution_vec.push_back(make(evolution_ids[i], from_game));
        }
    }
    
    bool base_pokemon_impl::is_fully_evolved() const
    {
        b_pkmn_vec_t evolution_vec;
        get_evolutions(evolution_vec);

        return (evolution_vec.begin() == evolution_vec.end());
    }
    
    unsigned int base_pokemon_impl::get_generation() const {return from_gen;}
    
    string base_pokemon_impl::get_species_name() const
    {
        switch(species_id)
        {
            case Species::NONE:
                return "None";

            case Species::INVALID:
                return "Invalid Pokemon";

            default:
                return database::get_species_name(species_id);
        }
    }

    unsigned int base_pokemon_impl::get_game_id() const {return from_game;}

    unsigned int base_pokemon_impl::get_pokemon_id() const {return pokemon_id;}

    unsigned int base_pokemon_impl::get_species_id() const {return species_id;}
} /* namespace pkmnsim */