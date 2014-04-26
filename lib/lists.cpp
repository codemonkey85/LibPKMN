/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/lists.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include <pokehack/pokestructs.h>
#include <pokelib/data_tables.h>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmn
{
    SQLite::Database db(get_database_path().c_str());

    void get_game_list(std::vector<std::string> &game_vec)
    {
        game_vec.clear();

        std::string query_str = "SELECT name FROM version_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep())
        {
            std::string game = query.getColumn(0);
            game_vec.push_back(game);
        }
    }

    void get_game_group_list(std::vector<std::string> &game_group_vec)
    {
        //Must be done manually, only really used for GamesComboBox
        game_group_vec.clear();

        game_group_vec.push_back("Red/Blue/Green");
        game_group_vec.push_back("Yellow");
        game_group_vec.push_back("Gold/Silver");
        game_group_vec.push_back("Crystal");
        game_group_vec.push_back("Ruby/Sapphire/Emerald");
        game_group_vec.push_back("Fire Red/Leaf Green");
        game_group_vec.push_back("Diamond/Pearl");
        game_group_vec.push_back("Platinum");
        game_group_vec.push_back("Heart Gold/Soul Silver");
        game_group_vec.push_back("Black/White");
        game_group_vec.push_back("Black 2/White 2");
    }

    void get_item_list(std::vector<std::string> &item_vec, unsigned int game)
    {
        item_vec.clear();
        std::vector<std::string> temp_vec;

        switch(game)
        {
            case Games::XD:
            case Games::EMERALD:
                //Gen 3 items appearing only in Emerald
                item_vec.push_back(items[374]);
                item_vec.push_back(items[373]);
            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                //Gen 3 items appearing only  in Fire Red, Leaf Green, and Emerald
                temp_vec.clear();
                for(int i = 348; i < 374; i++) temp_vec.push_back(items[i]);
                item_vec.insert(item_vec.begin(), temp_vec.begin(), temp_vec.end());
            case Games::COLOSSEUM:
            case Games::RUBY:
            case Games::SAPPHIRE:
                //Gen 3 items appearing in all Gen 3 games
                temp_vec.clear();
                for(int i = 0; i < 348; i++)
                    if(not (strncmp(items[i], "Nothing", 7) or strncmp(items[i], "???", 3)))
                        temp_vec.push_back(items[i]);
                item_vec.insert(item_vec.begin(), temp_vec.begin(), temp_vec.end());
                break;

            case Games::HEART_GOLD:
            case Games::SOUL_SILVER:
                //Gen 4 items appearing only in Heart Gold and Soul Silver
                for(int i = 468; i < 536; i++) item_vec.push_back(PokeLib::items[i]);
            case Games::PLATINUM:
                //Gen 4 items appearing only in Platinum, Heart Gold, and Soul Silver
                temp_vec.clear();
                temp_vec.push_back(PokeLib::items[111]);
                temp_vec.push_back(PokeLib::items[464]);
                temp_vec.push_back(PokeLib::items[465]);
                temp_vec.push_back(PokeLib::items[466]);
                item_vec.insert(item_vec.begin(), temp_vec.begin(), temp_vec.end());
            case Games::DIAMOND:
            case Games::PEARL:
                //Gen 4 items appearing in all Gen 4 games
                temp_vec.clear();
                for(int i = 0; i < 464; i++)
                    if(i != 111 and not (strncmp(PokeLib::items[i], "???", 3) or strncmp(PokeLib::items[i], "----", 4)))
                        temp_vec.push_back(PokeLib::items[i]);
                item_vec.insert(item_vec.begin(), temp_vec.begin(), temp_vec.end());
                break;

            case Games::BLACK:
            case Games::WHITE:
            {
                std::string query_string = "SELECT name FROM item_names WHERE item_id<670 AND local_language_id=9";
                SQLite::Statement query(db, query_string.c_str());
                while(query.executeStep())
                {
                    std::string result = query.getColumn(0); //Cannot do straight SQLite::Column to std::std::string conversion in push_back
                    item_vec.push_back(result);
                }
                break;
            }

            default: //For Black 2 and White 2, show all items, as well as for any invalid entry
            {
                std::string query_string = "SELECT name FROM item_names WHERE item_id AND local_language_id=9";
                SQLite::Statement query(db, query_string.c_str());
                while(query.executeStep())
                {
                    std::string result = query.getColumn(0); //Cannot do straight SQLite::Column to std::std::string conversion in push_back
                    item_vec.push_back(result);
                }
                break;
            }
        }
    }

    void PKMN_API get_pokedex_order(std::vector<std::pair<unsigned int, unsigned int> >& entry_list, unsigned int pokedex_id)
    {
        entry_list.clear();
        std::string query_string(str(boost::format("SELECT species_id,pokedex_number FROM pokemon_dex_numbers WHERE pokedex_id=%d")
                                     % pokedex_id));
        SQLite::Statement query(db, query_string.c_str());

        while(query.executeStep()) entry_list.push_back(std::make_pair(int(query.getColumn(0)), int(query.getColumn(1))));
    }

    void get_pokemon_list(std::vector<std::string> &pokemon_vec, unsigned int game)
    {
        pokemon_vec.clear();

        //Amount of Pokemon in generation correponding to game enum specified
        int bounds[] = {151,151,151,151,251,251,251,386,386,386,386,386,493,493,493,493,493,
                        649,649,386,386,649,649,151,251};
        bool multiple = false;

        std::string query_string = "SELECT id,species_id FROM pokemon WHERE id <= " + to_string(bounds[game]);
        SQLite::Statement query(db, query_string.c_str());

        boost::format form_format("%s (%s)");
        while(query.executeStep())
        {

            int pokemon_id = query.getColumn(0);
            int species_id = query.getColumn(1);

            std::string normal_name;

            //For the Nidorans, eliminate the multi-byte character
            if(species_id == 29) normal_name = "Nidoran (F)";
            else if(species_id == 32) normal_name = "Nidoran (M)";
            else
            {
                query_string = "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id=" + to_string(species_id);
                normal_name = std::string((const char*)db.execAndGet(query_string.c_str()));
            }

            query_string = "SELECT id,form_identifier FROM pokemon_forms WHERE form_identifier!='NULL' AND pokemon_id=" + to_string(pokemon_id);
            SQLite::Statement inner_query(db, query_string.c_str());

            while(inner_query.executeStep())
            {
                multiple = true;

                int form_id = inner_query.getColumn(0);
                std::string form_identifier = inner_query.getColumn(1);

                query_string = "SELECT form_name FROM pokemon_form_names WHERE local_language_id=9 AND pokemon_form_id=" + to_string(form_id);
                std::string form_name = std::string((const char*)db.execAndGet(query_string.c_str()));
                std::vector<std::string> form_halves;
                boost::split(form_halves, form_name, boost::is_any_of(" "));
                std::string full_form_name = (form_format % normal_name % form_halves[0]).str();

                //Manaual additions/removals
                switch(pokemon_id)
                {
                    case 172:
                        //Pichu appears in all Gen 2+ games, but Spiky-eared Pichu only appears in HG/SS
                        pokemon_vec.push_back("Pichu");
                        if(game == Games::HEART_GOLD or game == Games::SOUL_SILVER) pokemon_vec.push_back(full_form_name);
                        break;

                    case 201:
                        //Unown (?) and Unown (!) were introduced in Gen 3
                        if(form_id == 675 or form_id == 676)
                        {
                            if(game > Games::CRYSTAL) pokemon_vec.push_back(full_form_name);
                        }
                        else pokemon_vec.push_back(full_form_name);
                        break;

                    case 386:
                        //In Gen 3, different Deoxys forms only appear in certain games
                        if(game != Games::FIRE_RED and game != Games::LEAF_GREEN and game != Games::EMERALD)
                            pokemon_vec.push_back(full_form_name);
                        if(game != Games::RUBY and game != Games::SAPPHIRE and game != Games::LEAF_GREEN and game != Games::EMERALD)
                            pokemon_vec.push_back("Deoxys (Attack)");
                        if(game != Games::RUBY and game != Games::SAPPHIRE and game != Games::FIRE_RED and game != Games::EMERALD)
                            pokemon_vec.push_back("Deoxys (Defense)");
                        if(game != Games::RUBY and game != Games::SAPPHIRE and game != Games::FIRE_RED and game != Games::LEAF_GREEN)
                            pokemon_vec.push_back("Deoxys (Speed)");
                        break;

                    case 413:
                        pokemon_vec.push_back(full_form_name);
                        pokemon_vec.push_back("Wormadam (Sandy)");
                        pokemon_vec.push_back("Wormadam (Trash)");
                        break;

                    case 487:
                        //Giratina's Origin Forme first appeared in Platinum
                        pokemon_vec.push_back(full_form_name);
                        if(game > Games::PEARL) pokemon_vec.push_back("Giratina (Origin)");
                        break;

                    case 492:
                        //Shaymin's Sky Forme first appeared in Platinum
                        pokemon_vec.push_back(full_form_name);
                        if(game > Games::PEARL) pokemon_vec.push_back("Shaymin (Sky)");
                        break;

                    case 493:
                        //Arceus (???) only appears in Gen 4
                        if(form_halves[0] == "???")
                        {
                            if(game < Games::BLACK) pokemon_vec.push_back(full_form_name);
                        }
                        else pokemon_vec.push_back(full_form_name);
                        break;

                    case 550:
                        pokemon_vec.push_back(full_form_name);
                        pokemon_vec.push_back("Basculin (Blue-Striped)");
                        break;

                    case 555:
                        pokemon_vec.push_back(full_form_name);
                        pokemon_vec.push_back("Darmanitan (Zen)");
                        break;

                    case 641:
                    case 642:
                    case 645:
                        //Tornadus, Thunderus, and Landorus all have both Incarnate and Therian forms
                        pokemon_vec.push_back(full_form_name);
                        pokemon_vec.push_back((form_format % normal_name % "Therian").str());
                        break;

                    case 647:
                        //Keldeo's Resolute Forme first appeared in Black 2/White 2
                        pokemon_vec.push_back(full_form_name);
                        if(game > Games::WHITE) pokemon_vec.push_back("Keldeo (Resolute)");
                        break;

                    case 648:
                        pokemon_vec.push_back(full_form_name);
                        pokemon_vec.push_back("Meloetta (Pirouette)");
                        break;

                    case 649:
                        //Place standard Genesect first
                        if(form_halves[0] == "Douse")
                        {
                            pokemon_vec.push_back("Genesect");
                            pokemon_vec.push_back(full_form_name);
                        }
                        else pokemon_vec.push_back(full_form_name);
                        break;

                    default:
                        pokemon_vec.push_back(full_form_name);
                        break;
                }
            }
            if(multiple) multiple = false;
            else
            {
                //Rotom and Kyrem aren't listed listed as having multiple forms, so it must be manually done here
                switch(int(query.getColumn(1)))
                {
                    case 479:
                        pokemon_vec.push_back(normal_name);
                        if(game > Games::PEARL)
                        {
                            pokemon_vec.push_back("Rotom (Heat)");
                            pokemon_vec.push_back("Rotom (Wash)");
                            pokemon_vec.push_back("Rotom (Frost)");
                            pokemon_vec.push_back("Rotom (Fan)");
                            pokemon_vec.push_back("Rotom (Mow)");
                        }
                        break;

                    case 646:
                        pokemon_vec.push_back(normal_name);
                        if(game > Games::WHITE)
                        {
                            pokemon_vec.push_back("Kyurem (Black)");
                            pokemon_vec.push_back("Kyurem (White)");
                        }
                        break;

                    default:
                        pokemon_vec.push_back(normal_name);
                        break;
                }
            }
        }
    }

    void get_type_list(std::vector<std::string> &type_vec, unsigned int gen)
    {
        type_vec.clear();
    
        std::string query_string = "SELECT name FROM type_names WHERE local_language_id=9";

        SQLite::Statement type_names_query(db, query_string.c_str());
        while(type_names_query.executeStep())
        {
            std::string type = std::string((const char*)type_names_query.getColumn(0));
            if(not (gen == 1 and (type == "Steel" or type == "Dark")) and type != "???" and type != "Shadow")
            {
                type_vec.push_back(type);
            }
        }
    }

    void get_ability_list(std::vector<std::string> &ability_vec, unsigned int gen)
    {
        ability_vec.clear();
    
        std::string query_string = "SELECT id FROM abilities WHERE generation_id<=" + to_string(gen);
        
        SQLite::Statement query(db, query_string.c_str());
        while(query.executeStep())
        {
            query_string = "SELECT name FROM ability_names WHERE local_language_id=9 ability_id=" + to_string(query.getColumn(0));
            ability_vec.push_back(std::string((const char*)db.execAndGet(query_string.c_str())));
        }
    }

    void get_nature_list(std::vector<std::string> &nature_vec)
    {
        nature_vec.clear();

        std::string query_str = "SELECT name FROM nature_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep())
        {
            std::string nature = std::string((const char*)query.getColumn(0));
            nature_vec.push_back(nature);
        }
    }
    
    void get_pokemon_of_type(base_pokemon_vector &pkmn_vector, std::string type1, std::string type2, unsigned int gen, bool lax)
    {
        pkmn_vector.clear();

        std::string query_string;
        std::vector<int> applicable_ids;
        int pkmn_id, type1_id, type2_id;

        //Get type IDs
        query_string = "SELECT type_id FROM type_names WHERE name='" + type1 + "'";
        type1_id = int(db.execAndGet(query_string.c_str()));
        if(type2 != "None" and type2 != "Any")
        {
            query_string = "SELECT type_id FROM type_names WHERE name='" + type2 + "'";
            type2_id = int(db.execAndGet(query_string.c_str()));
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

                //Get generation ID to restrict list
                query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(species_id);
                int generation_id = db.execAndGet(query_string.c_str());
                if(generation_id <= gen)
                {
                    applicable_ids.push_back(pkmn_id);
                }
            }
        }
        else
        {

            //Get IDs of Pokémon matching first type
            std::vector<int> pkmn_ids;
            query_string = "SELECT pokemon_id FROM pokemon_types WHERE type_id=" + to_string(type1_id);
            SQLite::Statement pokemon_types_id_query(db, query_string.c_str());

            while(pokemon_types_id_query.executeStep()) pkmn_ids.push_back(pokemon_types_id_query.getColumn(0));

            std::vector<int> to_erase;
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
                std::string pkmn_name = db.execAndGet(query_string.c_str());

                //Get generation ID to restrict list
                query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(species_id);
                int generation_id = db.execAndGet(query_string.c_str());
                if(generation_id <= gen) applicable_ids.push_back(pkmn_ids[i]); //ID's that apply to final Pokemon
            }
        }

        //base_pokemon now takes a game ID in its constructor instead of a generation, but this
        //function doesn't discriminate between games in the same generation, so this array
        //guarantees that the given generation will use a game in that generation
        int game_id_from_gen[] = {0,1,4,7,13,17};
        
        for(unsigned int i = 0; i < applicable_ids.size(); i++)
        {
            //Manually correct for Magnemite and Magneton in Gen 1
            int final_species_id = database::get_species_id(applicable_ids[i]);
            if(not ((database::get_species_name(final_species_id) == "Magnemite" or
                     database::get_species_name(final_species_id) == "Magneton") and gen == 1))
            {
                base_pokemon::sptr b_pkmn = base_pokemon::make(database::get_species_id(applicable_ids[i]), game_id_from_gen[gen]);
                b_pkmn->repair(applicable_ids[i]);
                pkmn_vector.push_back(b_pkmn);
            }
        }
    }
} /* namespace pkmn */
