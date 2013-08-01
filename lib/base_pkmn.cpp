/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/lists.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "base_pkmn_gen1impl.hpp"
#include "base_pkmn_gen2impl.hpp"
#include "base_pkmn_gen345impl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
	base_pkmn::base_pkmn(string identifier, int gen)
	{
        from_gen = gen;
        database_identifier = identifier;
    
        SQLite::Database db(get_database_path().c_str());

        string query_string;

        //Fail if Pokémon's generation_id > specified gen
        query_string = "SELECT * FROM pokemon_species WHERE identifier='" + identifier + "'";
        SQLite::Statement pokemon_species_query(db, query_string.c_str());
        pokemon_species_query.executeStep();
        int gen_id = pokemon_species_query.getColumn(2); //generation_id
        if(gen_id > gen)
        {
            string error_message = identifier + " not present in Generation " + to_string(gen) + ".";
            throw runtime_error(error_message.c_str());
        }

        //After Pokemon verified as valid, get necessary ID's
        species_id = pokemon_species_query.getColumn(0); //id
        query_string = "SELECT id FROM pokemon WHERE species_id=" + to_string(species_id);
        pkmn_id = db.execAndGet(query_string.c_str());
        query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_id) + " AND slot=1";
        int type1_id = db.execAndGet(query_string.c_str(), identifier);
        query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_id) + " AND slot=2";
        SQLite::Statement type2_query(db, query_string.c_str());
        if(type2_query.executeStep()) type2_id = type2_query.getColumn(0);
        else type2_id = -1;

        icon_path = fs::path(fs::path(get_images_dir()) / "icons" / str(boost::format("%d.png") % species_id)).string();
        male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / str(boost::format("%d.png") % species_id)).string();
        female_sprite_path = male_sprite_path;
        male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / str(boost::format("%d.png") % species_id)).string();
        female_shiny_sprite_path = male_shiny_sprite_path;

        repair(pkmn_id);
	}
	
    base_pkmn::sptr base_pkmn::make(string identifier, int gen)
    {
        try
        {
            //Match database's identifier format
            identifier = database::to_database_format(identifier);

            if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

            switch(gen)
            {
                case 1:
                    return sptr(new base_pkmn_gen1impl(identifier));

                case 2:
                    return sptr(new base_pkmn_gen2impl(identifier));

                default:
                    return sptr(new base_pkmn_gen345impl(identifier, gen));
            }
        }
        catch(const exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    string base_pkmn::get_species_name(void)
    {
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id=" + to_string(species_id);
        return db.execAndGetStr(query_string.c_str(), "name");
    }

    int base_pkmn::get_nat_pokedex_num(void) {return species_id;}

    dict<int, std::string> base_pkmn::get_types(void)
    {
        dict<int, std::string> type_dict;
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT name FROM type_names WHERE type_id=" + to_string(type1_id);
        type_dict[0] = db.execAndGetStr(query_string.c_str(), "name");
        if(type2_id != -1)
        {
            query_string = "SELECT name FROM type_names WHERE type_id=" + to_string(type2_id);
            type_dict[1] = db.execAndGetStr(query_string.c_str(), "name");
        }
        else type_dict[1] = "None";

        return type_dict;
    }

    double base_pkmn::get_height(void)
    {
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT height FROM pokemon WHERE id=" + to_string(pkmn_id);
        return (double(db.execAndGet(query_string.c_str())) / 10.0);
    }

    double base_pkmn::get_weight(void)
    {
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT weight FROM pokemon WHERE id=" + to_string(pkmn_id);
        return (double(db.execAndGet(query_string.c_str())) / 10.0);
    }

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

            evolution_vec.push_back(make(evol_identifier, from_gen));
        }
	}
	
    bool base_pkmn::is_fully_evolved()
    {
        vector<sptr> evolution_vec;
        get_evolutions(evolution_vec);

        return (evolution_vec.begin() == evolution_vec.end());
    }

    int base_pkmn::get_generation(void) {return from_gen;}
    int base_pkmn::get_pokemon_id(void) {return pkmn_id;}
    int base_pkmn::get_species_id(void) {return species_id;}
    string base_pkmn::get_icon_path(void) {return icon_path;}

    //Manually set Pokemon form
    void base_pkmn::set_form(int form)
    {
        if(species_id == 201)
        {
            if(form >= 1 and form <= 26)
            {
                char letter = form + 96;
                string basename = str(boost::format("201-%c.png") % letter);
                icon_path = fs::path(fs::path(get_images_dir()) / "icons" / basename).string();
                male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / basename).string();
                female_sprite_path = male_sprite_path;
                male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / basename).string();
                female_shiny_sprite_path = male_sprite_path;
            }
            else if(form == Forms::Unown::QUESTION)
            {
                icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "201-question.png").string();
                male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "201-question.png").string();
                female_sprite_path = male_sprite_path;
                male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "201-question.png").string();
                female_shiny_sprite_path = male_sprite_path;
            }
            else if(form == Forms::Unown::EXCLAMATION)
            {
                icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "201-exclamation.png").string();
                male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "201-exclamation.png").string();
                female_sprite_path = male_sprite_path;
                male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "201-exclamation.png").string();
                female_shiny_sprite_path = male_sprite_path;
            }
            else
            {
                cerr << "Unown can only be an alphabet letter, ? or !." << endl;
                exit(EXIT_FAILURE);
            }
            
        }
        else if(species_id = 351)
        {
            switch(form)
            {
                case Forms::Castform::NORMAL:
                    type1_id = 1;
                    type2_id = -1;
                    pkmn_id = 351;

                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "351.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Castform::SUNNY:
                    type1_id = 10;
                    type2_id = -1;
                    pkmn_id = 662;

                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "351-sunny.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351-sunny.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351-sunny.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Castform::RAINY:
                    type1_id = 11;
                    type2_id = -1;
                    pkmn_id = 663;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "351-rainy.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351-rainy.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351-rainy.png").string();
                    female_shiny_sprite_path = female_shiny_sprite_path;
                    break;

                case Forms::Castform::SNOWY:
                    type1_id = 15;
                    type2_id = -1;
                    pkmn_id = 664;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "351-snowy.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351-snowy.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "351-snowy.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Castform has the following forms: Normal, Sunny, Rainy, Snowy." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 386)
        {
            switch(form)
            {
                case Forms::Deoxys::NORMAL:
                    pkmn_id = 386;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "386.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "386.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "386.png").string();
                    break;

                case Forms::Deoxys::ATTACK:
                    pkmn_id = 650;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "386-attack.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "386-attack.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "386-attack.png").string();
                    female_shiny_sprite_path = female_sprite_path;

                case Forms::Deoxys::DEFENSE:
                    pkmn_id = 651;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "386-defense.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "386-defense.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "386-defense.png").string();
                    female_shiny_sprite_path = female_sprite_path;

                case Forms::Deoxys::SPEED:
                    pkmn_id = 652;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "386-speed.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "386-speed.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "386-speed.png").string();
                    female_shiny_sprite_path = female_sprite_path;

                default:
                    cerr << "Deoxys has the following forms: Normal, Attack, Defense, Speed." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 412)
        {
            switch(form)
            {
                case Forms::Burmy::PLANT_CLOAK:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "412-plant.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "412-plant.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "412-plant.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Burmy::SANDY_CLOAK:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "412-sandy.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "412-sandy.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "412-sandy.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Burmy::TRASH_CLOAK:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "412-trash.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "412-trash.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "412-trash.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Burmy has the following forms: Plant Cloak, Sandy Cloak, Trash Cloak." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 413)
        {
            switch(form)
            {
                case Forms::Wormadam::PLANT_CLOAK:
                    type1_id = 7;
                    type2_id = 12;
                    pkmn_id = 413;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "413-plant.png").string();
                    female_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "female" / "413-plant.png").string();
                    male_sprite_path = female_sprite_path; //Will never be used
                    female_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "female" / "413-plant.png").string();
                    male_shiny_sprite_path = female_shiny_sprite_path; //Will never be used

                case Forms::Wormadam::SANDY_CLOAK:
                    type1_id = 7;
                    type2_id = 5;
                    pkmn_id = 653;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "413-sandy.png").string();
                    female_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "female" / "413-sandy.png").string();
                    male_sprite_path = female_sprite_path; //Will never be used
                    female_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "female" / "413-sandy.png").string();
                    male_shiny_sprite_path = female_shiny_sprite_path; //Will never be used

                case Forms::Wormadam::TRASH_CLOAK:
                    type1_id = 7;
                    type2_id = 9;
                    pkmn_id = 654;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "413-trash.png").string();
                    female_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "female" / "413-trash.png").string();
                    male_sprite_path = female_sprite_path; //Will never be used
                    female_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "female" / "413-trash.png").string();
                    male_shiny_sprite_path = female_shiny_sprite_path; //Will never be used

                default:
                    cerr << "Wormadam has the following forms: Plant Cloak, Sandy Cloak, Trash Cloak." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 421)
        {
            switch(form)
            {
                case Forms::Cherrim::OVERCAST:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "421-overcast.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "421-overcast.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "421-overcast.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Cherrim::SUNSHINE:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "421-sunshine.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "421-sunshine.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "421-sunshine.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Cherrim has the following forms: Overcast, Sunshine." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 422)
        {
            switch(form)
            {
                case Forms::Shellos::WEST_SEA:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "422-west.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "422-west.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "422-west.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Shellos::EAST_SEA:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "422-east.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "422-east.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "422-east.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Shellos has the following forms: West Sea, East Sea." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 423)
        {
            switch(form)
            {
                case Forms::Gastrodon::WEST_SEA:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "423-west.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "423-west.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "423-west.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Gastrodon::EAST_SEA:
                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "423-east.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "423-east.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "423-east.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Gastrodon has the following forms: West Sea, East Sea." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 479)
        {
            switch(form)
            {
                case Forms::Rotom::NORMAL:
                    type1_id = 13;
                    type2_id = 8;
                    pkmn_id = 479;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "479.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "479.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "479.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Rotom::HEAT:
                    type1_id = 13;
                    type2_id = 10;
                    pkmn_id = 657;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "479-heat.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "479-heat.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "479-heat.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Rotom::WASH:
                    type1_id = 13;
                    type2_id = 11;
                    pkmn_id = 658;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "479-wash.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "479-wash.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "479-wash.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Rotom::FROST:
                    type1_id = 13;
                    type2_id = 15;
                    pkmn_id = 659;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "479-frost.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "479-frost.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "479-frost.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Rotom::FAN:
                    type1_id = 13;
                    type2_id = 3;
                    pkmn_id = 660;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "479-fan.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "479-fan.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "479-fan.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Rotom::MOW:
                    type1_id = 13;
                    type2_id = 12;
                    pkmn_id = 661;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "479-mow.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "479-mow.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "479-mow.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Rotom has the following forms: Normal, Heat, Wash, Front, Fan, Mow." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 487)
        {
            switch(form)
            {
                case Forms::Giratina::ALTERED:
                    pkmn_id = 487;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "487-altered.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "487-altered.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "487-altered.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Giratina::ORIGIN:
                    pkmn_id = 656;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "487-origin.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "487-origin.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "487-origin.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Giratina has the following forms: Altered, Origin." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 492)
        {
            switch(form)
            {
                case Forms::Shaymin::LAND:
                    type1_id = 12;
                    type2_id = -1;
                    pkmn_id = 492;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "492-land.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "492-land.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "492-land.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Shaymin::SKY:
                    type1_id = 12;
                    type2_id = 4;
                    pkmn_id = 655;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "492-sky.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "492-sky.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "492-sky.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Shaymin has the following forms: Land, Sky." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 493)
        {
            switch(form)
            {
                case Forms::Arceus::NORMAL:
                    set_form("Normal");
                    break;

                case Forms::Arceus::FIRE:
                    set_form("Fire");
                    break;

                case Forms::Arceus::WATER:
                    set_form("Water");
                    break;

                case Forms::Arceus::ELECTRIC:
                    set_form("Electric");
                    break;

                case Forms::Arceus::GRASS:
                    set_form("Grass");
                    break;

                case Forms::Arceus::ICE:
                    set_form("Ice");
                    break;

                case Forms::Arceus::FIGHTING:
                    set_form("Fighting");
                    break;

                case Forms::Arceus::POISON:
                    set_form("Poison");
                    break;

                case Forms::Arceus::GROUND:
                    set_form("Ground");
                    break;

                case Forms::Arceus::FLYING:
                    set_form("Flying");
                    break;

                case Forms::Arceus::PSYCHIC:
                    set_form("Psychic");
                    break;

                case Forms::Arceus::BUG:
                    set_form("Bug");
                    break;

                case Forms::Arceus::ROCK:
                    set_form("Rock");
                    break;

                case Forms::Arceus::GHOST:
                    set_form("Ghost");
                    break;

                case Forms::Arceus::DRAGON:
                    set_form("Dragon");
                    break;

                case Forms::Arceus::DARK:
                    set_form("Dark");
                    break;

                case Forms::Arceus::STEEL:
                    set_form("Steel");
                    break;

                default:
                    cerr << "Arceus can only be set to one of the 17 types." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 550)
        {
            switch(form)
            {
                case Forms::Basculin::RED_STRIPED:
                    pkmn_id = 550;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "550-red-striped.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "550-red-striped.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "550-red-striped.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Basculin::BLUE_STRIPED:
                    pkmn_id = 665;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "550-blue-striped.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "550-blue-striped.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "550-blue-striped.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Basculin has the following forms: Red-Striped, Blue-Striped." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 555)
        {
            switch(form)
            {
                case Forms::Darmanitan::STANDARD:
                    pkmn_id = 555;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "555-standard.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "555-standard.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "555-standard.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Darmanitan::ZEN:
                    pkmn_id = 666;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "555-zen.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "555-zen.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "555-zen.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Darmanitan has the following forms: Standard, Zen." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 585)
        {
            switch(form)
            {
                case Forms::Deerling::SPRING:
                    set_form("Spring");
                    break;

                case Forms::Deerling::SUMMER:
                    set_form("Summer");
                    break;

                case Forms::Deerling::AUTUMN:
                    set_form("Autumn");
                    break;

                case Forms::Deerling::WINTER:
                    set_form("Winter");
                    break;

                default:
                    cerr << "Deerling has the following forms: Spring, Summer, Fall, Winter." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 586)
        {
            switch(form)
            {
                case Forms::Sawsbuck::SPRING:
                    set_form("Spring");
                    break;

                case Forms::Sawsbuck::SUMMER:
                    set_form("Summer");
                    break;

                case Forms::Sawsbuck::AUTUMN:
                    set_form("Autumn");
                    break;

                case Forms::Sawsbuck::WINTER:
                    set_form("Winter");
                    break;

                default:
                    cerr << "Sawsbuck has the following forms: Spring, Summer, Fall, Winter." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 641)
        {
            switch(form)
            {
                case Forms::Tornadus::INCARNATE:
                    pkmn_id = 641;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "641-incarnate.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "641-incarnate.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "641-incarnate.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Tornadus::THERIAN:
                    pkmn_id = 668;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "641-therian.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "641-therian.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "641-therian.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Tornadus has the following forms: Incarnate, Therian." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 642)
        {
            switch(form)
            {
                case Forms::Thundurus::INCARNATE:
                    pkmn_id = 642;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "642-incarnate.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "642-incarnate.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "642-incarnate.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Thundurus::THERIAN:
                    pkmn_id = 669;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "642-therian.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "642-therian.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "642-therian.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Thundurus has the following forms: Incarnate, Therian." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 645)
        {
            switch(form)
            {
                case Forms::Landorus::INCARNATE:
                    pkmn_id = 645;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "645-incarnate.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "645-incarnate.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "645-incarnate.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Landorus::THERIAN:
                    pkmn_id = 670;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "645-therian.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "645-therian.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "645-therian.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Landorus has the following forms: Incarnate, Therian." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 646)
        {
            switch(form)
            {
                case Forms::Kyurem::NORMAL:
                    pkmn_id = 646;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "646.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "646.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "646.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Kyurem::BLACK:
                    pkmn_id = 671;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "646-black.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "646-black.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "646-black.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Kyurem::WHITE:
                    pkmn_id = 672;

                    icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / "646-white.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "646-white.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / "646-white.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Kyurem has the following forms: Normal, Black, White." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 647)
        {
            switch(form)
            {
                case Forms::Keldeo::ORDINARY:
                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "647-ordinary.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "647-ordinary.png").string();
                    female_sprite_path = male_sprite_path;
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "647-ordinary.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Keldeo::RESOLUTE:
                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "647-resolute.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "647-resolute.png").string();
                    female_sprite_path = male_sprite_path;
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "647-resolute.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Keldeo has the following forms: Ordinary, Resolute." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 647)
        {
            switch(form)
            {
                case Forms::Meloetta::ARIA:
                    pkmn_id = 647;

                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "648-aria.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "648-aria.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "648-aria.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Meloetta::PIROUETTE:
                    pkmn_id = 673;

                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "648-pirouette.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "648-pirouette.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "648-pirouette.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Meloetta has the following forms: Aria, Pirouette." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 649)
        {
            switch(form)
            {
                case Forms::Genesect::NORMAL:
                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "649.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "649.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "649.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Genesect::SHOCK_DRIVE:
                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "649-shock.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "649-shock.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "649-shock.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Genesect::BURN_DRIVE:
                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "649-burn.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "649-burn.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "649-burn.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Genesect::CHILL_DRIVE:
                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "649-chill.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "649-chill.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "649-chill.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                case Forms::Genesect::DOUSE_DRIVE:
                    icon_path = fs::path(fs::path(get_images_dir()) / "icons" / "649-douse.png").string();
                    male_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "649-douse.png").string();
                    female_sprite_path = male_sprite_path;
                    male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "sprites" / "shiny" / "649-douse.png").string();
                    female_shiny_sprite_path = male_shiny_sprite_path;
                    break;

                default:
                    cerr << "Genesect has the following forms: Normal, Shock Drive, Burn Drive, Chill Drive, Douse Drive." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        else
        {
            cerr << "This Pokemon has no alternate forms." << endl;
            exit(EXIT_FAILURE);
        }
    }

    //Better for SWIG, which doesn't see the enums
    void base_pkmn::set_form(string form)
    {
        if(species_id == 201)
        {
            if(form.size() != 1)
            {
                cerr << "Unown can only be an alphabet letter, ? or !." << endl;
                exit(EXIT_FAILURE);
            }
            else
            {
                char letter = form.c_str()[0];
                if(tolower(letter) >= 'a' and tolower(letter) <= 'z') set_form(int(letter)-96);
                else if(letter == '?') set_form(Forms::Unown::QUESTION);
                else if(letter == '!') set_form(Forms::Unown::EXCLAMATION);
                else
                {
                    cerr << "Unown can only be an alphabet letter, ? or !." << endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        else if(species_id == 351)
        {
            if(form == "Normal") set_form(Forms::Castform::NORMAL);
            else if(form == "Sunny") set_form(Forms::Castform::SUNNY);
            else if(form == "Rainy") set_form(Forms::Castform::RAINY);
            else if(form == "Snowy") set_form(Forms::Castform::SNOWY);
            else
            {
                cerr << "Castform has the following forms: Normal, Sunny, Rainy, Snowy." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 386)
        {
            if(form == "Normal") set_form(Forms::Deoxys::NORMAL);
            else if(form == "Attack") set_form(Forms::Deoxys::ATTACK);
            else if(form == "Defense") set_form(Forms::Deoxys::DEFENSE);
            else if(form == "Speed") set_form(Forms::Deoxys::SPEED);
            else
            {
                cerr << "Deoxys has the following forms: Normal, Attack, Defense, and Speed." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 412)
        {
            if(form == "Plant Cloak") set_form(Forms::Burmy::PLANT_CLOAK);
            else if(form == "Sandy Cloak") set_form(Forms::Burmy::SANDY_CLOAK);
            else if(form == "Trash Cloak") set_form(Forms::Burmy::TRASH_CLOAK);
            else
            {
                cerr << "Burmy has the following forms: Plant Cloak, Sandy Cloak, Trash Cloak." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 413)
        {
            if(form == "Plant Cloak") set_form(Forms::Wormadam::PLANT_CLOAK);
            else if(form == "Sandy Cloak") set_form(Forms::Wormadam::SANDY_CLOAK);
            else if(form == "Trash Cloak") set_form(Forms::Wormadam::TRASH_CLOAK);
            else
            {
                cerr << "Wormadam has the following forms: Plant Cloak, Sandy Cloak, Trash Cloak." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 421)
        {
            if(form == "Overcast") set_form(Forms::Cherrim::OVERCAST);
            else if(form == "Sunshine") set_form(Forms::Cherrim::SUNSHINE);
            else
            {
                cerr << "Cherrim has the following forms: Overcast, Sunshine." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 422)
        {
            if(form == "West Sea") set_form(Forms::Shellos::WEST_SEA);
            else if(form == "East Sea") set_form(Forms::Shellos::EAST_SEA);
            else
            {
                cerr << "Shellos has the following forms: West Sea, East Sea." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 423)
        {
            if(form == "West Sea") set_form(Forms::Gastrodon::WEST_SEA);
            else if(form == "East Sea") set_form(Forms::Gastrodon::EAST_SEA);
            else
            {
                cerr << "Gastrodon has the following forms: West Sea, East Sea." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 479)
        {
            if(form == "Normal") set_form(Forms::Rotom::NORMAL);
            else if(form == "Heat") set_form(Forms::Rotom::HEAT);
            else if(form == "Frost") set_form(Forms::Rotom::FROST);
            else if(form == "Wash") set_form(Forms::Rotom::WASH);
            else if(form == "Fan") set_form(Forms::Rotom::FAN);
            else if(form == "Mow") set_form(Forms::Rotom::MOW);
            else
            {
                cerr << "Rotom has the following forms: Normal, Heat, Wash, Front, Fan, Mow." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 487)
        {
            if(form == "Altered") set_form(Forms::Giratina::ALTERED);
            else if(form == "Origin") set_form(Forms::Giratina::ORIGIN);
            else
            {
                cerr << "Giratina has the following forms: Altered, Origin." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 492)
        {
            if(form == "Land") set_form(Forms::Shaymin::LAND);
            else if(form == "Sky") set_form(Forms::Shaymin::SKY);
            else
            {
                cerr << "Shaymin has the following forms: Land, Sky." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 493)
        {
            vector<string> type_vec;
            get_type_list(type_vec, 4);

            if(find(type_vec.begin(), type_vec.end(), form) != type_vec.end())
            {
                type1_id = database::get_type_id_from_name(form);
                type2_id = -1;
                transform(form.begin(), form.end(), form.begin(), ::tolower);
                string basename = str(boost::format("493-%s.png") % form);

                icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / basename.c_str()).string();
                male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / basename.c_str()).string();
                female_sprite_path = male_sprite_path;
                male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / basename.c_str()).string();
                female_shiny_sprite_path = male_shiny_sprite_path;
            }
            else
            {
                cerr << "Arceus can only be set to one of the 17 types." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 550)
        {
            if(form == "Red-Striped") set_form(Forms::Basculin::RED_STRIPED);
            if(form == "Blue-Striped") set_form(Forms::Basculin::BLUE_STRIPED);
            else
            {
                cerr << "Basculin has the following forms: Red-Striped, Blue-Striped." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 555)
        {
            if(form == "Standard") set_form(Forms::Darmanitan::STANDARD);
            if(form == "Zen") set_form(Forms::Darmanitan::ZEN);
            else
            {
                cerr << "Darmanitan has the following forms: Red-Striped, Blue-Striped." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 585)
        {
            if(form != "Spring" and form != "Summer" and form != "Autumn" and form != "Winter")
            {
                transform(form.begin(), form.end(), form.begin(), ::tolower);
                string basename = str(boost::format("585-%s.png") % form);
                icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / basename.c_str()).string();
                male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / basename.c_str()).string();
                female_sprite_path = male_sprite_path;
                male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / basename.c_str()).string();
                female_shiny_sprite_path = male_shiny_sprite_path;
            }
            else
            {
                cerr << "Deerling has the following forms: Spring, Summer, Fall, Winter." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 586)
        {
            if(form != "Spring" and form != "Summer" and form != "Autumn" and form != "Winter")
            {
                transform(form.begin(), form.end(), form.begin(), ::tolower);
                string basename = str(boost::format("586-%s.png") % form);
                icon_path = fs::path(fs::path(get_images_dir().c_str()) / "icons" / basename.c_str()).string();
                male_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / basename.c_str()).string();
                female_sprite_path = male_sprite_path;
                male_shiny_sprite_path = fs::path(fs::path(get_images_dir().c_str()) / "sprites" / "shiny" / basename.c_str()).string();
                female_shiny_sprite_path = male_shiny_sprite_path;
            }
            else
            {
                cerr << "Deerling has the following forms: Spring, Summer, Fall, Winter." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 641)
        {
            if(form == "Incarnate") set_form(Forms::Tornadus::INCARNATE);
            if(form == "Therian") set_form(Forms::Tornadus::THERIAN);
            else
            {
                cerr << "Tornadus has the following forms: Incarnate, Therian." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 642)
        {
            if(form == "Incarnate") set_form(Forms::Thundurus::INCARNATE);
            if(form == "Therian") set_form(Forms::Thundurus::THERIAN);
            else
            {
                cerr << "Thundurus has the following forms: Incarnate, Therian." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 645)
        {
            if(form == "Incarnate") set_form(Forms::Landorus::INCARNATE);
            if(form == "Therian") set_form(Forms::Landorus::THERIAN);
            else
            {
                cerr << "Landorus has the following forms: Incarnate, Therian." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 646)
        {
            if(form == "Normal") set_form(Forms::Kyurem::NORMAL);
            else if(form == "Black") set_form(Forms::Kyurem::BLACK);
            else if(form == "White") set_form(Forms::Kyurem::WHITE);
            else
            {
                cerr << "Kyurem has the following forms: Normal, Black, White." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 647)
        {
            if(form == "Ordinary") set_form(Forms::Keldeo::ORDINARY);
            else if(form == "Resolute") set_form(Forms::Keldeo::RESOLUTE);
            else
            {
                cerr << "Keldeo has the following forms: Ordinary, Resolute." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 648)
        {
            if(form == "Aria") set_form(Forms::Meloetta::ARIA);
            else if(form == "Pirouette") set_form(Forms::Meloetta::PIROUETTE);
            else
            {
                cerr << "Meloetta has the following items: Aria, Pirouette." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(species_id == 649)
        {
            if(form == "Normal") set_form(Forms::Genesect::NORMAL);
            if(form == "Shock Drive") set_form(Forms::Genesect::SHOCK_DRIVE);
            if(form == "Burn Drive") set_form(Forms::Genesect::BURN_DRIVE);
            if(form == "Chill Drive") set_form(Forms::Genesect::CHILL_DRIVE);
            if(form == "Douse Drive") set_form(Forms::Genesect::DOUSE_DRIVE);
            else
            {
                cerr << "Genesect has the following forms: Normal, Shock Drive, Burn Drive, Chill Drive, Douse Drive." << endl;
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            cerr << "This Pokemon has no alternate forms." << endl;
            exit(EXIT_FAILURE);
        }
    }

    void base_pkmn::repair(int id)
    {
        switch(id)
        {
            case 650: //Deoxys - Attack Forme
                set_form(Forms::Deoxys::ATTACK);
                break;

            case 651: //Deoxys - Defense Forme
                set_form(Forms::Deoxys::DEFENSE);
                break;

            case 652: //Deoxys - Speed Forme
                set_form(Forms::Deoxys::SPEED);
                break;

            case 653: //Wormadam - Sandy Cloak
                set_form(Forms::Wormadam::SANDY_CLOAK);
                break;

            case 654: //Wormadam - Trash Cloak
                set_form(Forms::Wormadam::TRASH_CLOAK);
                break;

            case 655: //Shaymin - Sky Forme
                set_form(Forms::Shaymin::SKY);
                break;

            case 656: //Giratina - Origin Forme
                set_form(Forms::Giratina::ORIGIN);
                break;

            case 657: //Rotom - Heat
                set_form(Forms::Rotom::HEAT);
                break;

            case 658: //Rotom - Wash
                set_form(Forms::Rotom::WASH);
                break;

            case 659: //Rotom - Frost
                set_form(Forms::Rotom::FROST);
                break;

            case 660: //Rotom - Fan
                set_form(Forms::Rotom::FAN);
                break;

            case 661: //Rotom - Mow
                set_form(Forms::Rotom::MOW);
                break;

            case 662: //Castform - Sunny
                set_form(Forms::Castform::SUNNY);
                break;

            case 663: //Castform - Rainy
                set_form(Forms::Castform::RAINY);
                break;

            case 664: //Castform - Snowy
                set_form(Forms::Castform::SNOWY);
                break;

            case 665: //Basculin - Blue-Striped
                set_form(Forms::Basculin::BLUE_STRIPED);
                break;

            case 666: //Darmanitan - Zen
                set_form(Forms::Darmanitan::ZEN);
                break;

            case 667: //Meloetta - Pirouette
                set_form(Forms::Meloetta::PIROUETTE);
                break;

            case 668: //Tornadus - Therian
                set_form(Forms::Tornadus::THERIAN);
                break;

            case 669: //Thundurus - Therian
                set_form(Forms::Thundurus::THERIAN);
                break;

            case 670: //Landorus - Therian
                set_form(Forms::Landorus::THERIAN);
                break;

            case 671: //Kyurem - Black
                set_form(Forms::Kyurem::BLACK);
                break;

            case 672: //Kyurem - White
                set_form(Forms::Kyurem::WHITE);
                break;

            case 673: //Keldeo - Resolute
                set_form(Forms::Keldeo::RESOLUTE);
                break;

            default: //Nothing to do
                break;
        }
    }

    void get_pkmn_of_type(vector<base_pkmn::sptr> &pkmn_vector, string type1, string type2, int gen, bool lax)
    {
        pkmn_vector.clear();

        SQLite::Database db(get_database_path().c_str());
        string query_string;
        vector<string> names;
        vector<int> applicable_ids;
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
                if(generation_id <= gen)
                {
                    names.push_back(pkmn_name);
                    applicable_ids.push_back(pkmn_id);
                }
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
            for(unsigned int i = to_erase.size()-1; i > 0; i--) pkmn_ids.erase(pkmn_ids.begin() + to_erase[i]);
            pkmn_ids.erase(pkmn_ids.begin() + to_erase[0]);

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
                if(generation_id <= gen)
                {
                    applicable_ids.push_back(pkmn_ids[i]); //ID's that apply to final Pokemon
                    names.push_back(pkmn_name);
                }
            }
        }

        for(unsigned int i = 0; i < names.size(); i++)
        {
            //Manually correct for Magnemite and Magneton in Gen 1
            if(not ((names[i] == "magnemite" or names[i] == "magneton") and gen == 1))
            {
                base_pkmn::sptr b_pkmn = base_pkmn::make(names[i], gen);
                b_pkmn->repair(applicable_ids[i]);
                pkmn_vector.push_back(b_pkmn);
            }
        }
    }

    vector<base_pkmn::sptr> get_pkmn_list_of_type(string type1, string type2, int gen, bool lax)
    {
        vector<base_pkmn::sptr> b_pkmn_vec;
        get_pkmn_of_type(b_pkmn_vec, type1, type2, gen, lax);
        return b_pkmn_vec;
    }
}
