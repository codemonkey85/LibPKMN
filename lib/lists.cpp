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

#include <pkmnsim/enums.hpp>
#include <pkmnsim/lists.hpp>
#include <pkmnsim/paths.hpp>

#include <pokehack/pokestructs.h>
#include <pokelib/data_tables.h>

#include <sqlitecpp/SQLiteCPP.h>

using namespace std;

namespace pkmnsim
{
    void get_game_list(vector<string>& game_vec)
    {
        game_vec.clear();

        SQLite::Database db(get_database_path().c_str());
        string query_str = "SELECT name FROM version_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep())
        {
            string game = query.getColumn(0);
            game_vec.push_back(game);
        }
    }

    void get_game_group_list(vector<string>& game_group_vec)
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

    void get_item_list(vector<string> &item_vec, int game)
    {
        item_vec.clear();
        vector<string> temp_vec;

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
                    if(items[i] != "Nothing" and items[i] != "???")
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
                    if(i != 111 and PokeLib::items[i] != "???" and PokeLib::items[i] != "----")
                        temp_vec.push_back(PokeLib::items[i]);
                item_vec.insert(item_vec.begin(), temp_vec.begin(), temp_vec.end());
                break;

            case Games::BLACK:
            case Games::WHITE:
            {
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT name FROM item_names WHERE item_id<670 AND local_language_id=9";
                SQLite::Statement query(db, query_string.c_str());
                while(query.executeStep()) item_vec.push_back(query.getColumn(0));
                break;
            }

            default: //For Black 2 and White 2, show all items, as well as for any invalid entry
            {
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT name FROM item_names WHERE item_id AND local_language_id=9";
                SQLite::Statement query(db, query_string.c_str());
                while(query.executeStep()) item_vec.push_back(query.getColumn(0));
                break;
                break;
            }
        }
    }

    void get_pokemon_list(vector<string>& pokemon_vec, int game)
    {
        pokemon_vec.clear();

        //Amount of Pokemon in generation correponding to game enum specified
        int bounds[] = {151,151,151,151,251,251,251,386,386,386,386,386,493,493,493,493,493,
                        649,649,386,386,649,649,151,251};
        bool multiple = false;

        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT id,species_id FROM pokemon WHERE id <= " + to_string(bounds[game]);
        SQLite::Statement query(db, query_string.c_str());

        boost::format form_format("%s (%s)");
        while(query.executeStep())
        {

            int pokemon_id = query.getColumn(0);
            int species_id = query.getColumn(1);

            string normal_name;

            //For the Nidorans, eliminate the multi-byte character
            if(species_id == 29) normal_name = "Nidoran (F)";
            else if(species_id == 32) normal_name = "Nidoran (M)";
            else
            {
                query_string = "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id=" + to_string(species_id);
                normal_name = db.execAndGetStr(query_string.c_str(), "name");
            }

            query_string = "SELECT id,form_identifier FROM pokemon_forms WHERE form_identifier!='NULL' AND pokemon_id=" + to_string(pokemon_id);
            SQLite::Statement inner_query(db, query_string.c_str());

            while(inner_query.executeStep())
            {
                multiple = true;

                int form_id = inner_query.getColumn(0);
                string form_identifier = inner_query.getColumn(1);

                query_string = "SELECT form_name FROM pokemon_form_names WHERE local_language_id=9 AND pokemon_form_id=" + to_string(form_id);
                string form_name = db.execAndGet(query_string.c_str());
                vector<string> form_halves;
                boost::split(form_halves, form_name, boost::is_any_of(" "));
                string full_form_name = (form_format % normal_name % form_halves[0]).str();

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

    void get_type_list(vector<string>& type_vec, int gen)
    {
        type_vec.clear();
    
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT name FROM type_names WHERE local_language_id=9";

        SQLite::Statement type_names_query(db, query_string.c_str());
        while(type_names_query.executeStep())
        {
            string type = type_names_query.getColumn(0);
            if(not (gen == 1 and (type == "Steel" or type == "Dark")) and type != "???" and type != "Shadow")
            {
                type_vec.push_back(type);
            }
        }
    }

    void get_ability_list(vector<string>& ability_vec, int gen)
    {
        ability_vec.clear();
    
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT id FROM abilities WHERE generation_id<=" + to_string(gen);
        
        SQLite::Statement query(db, query_string.c_str());
        while(query.executeStep())
        {
            query_string = "SELECT name FROM ability_names WHERE local_language_id=9 ability_id=" + to_string(query.getColumn(0));
            ability_vec.push_back(db.execAndGetStr(query_string.c_str(), ""));
        }
    }

    void get_nature_list(vector<string>& nature_vec)
    {
        nature_vec.clear();

        SQLite::Database db(get_database_path().c_str());
        string query_str = "SELECT name FROM nature_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep())
        {
            string nature = query.getColumn(0);
            nature_vec.push_back(nature);
        }
    }

    //List functions that pass by value, for SWIG's use
    vector<string> get_game_vec()
    {
        vector<string> game_list;
        get_game_list(game_list);
        return game_list;
    }

    vector<string> get_game_group_vec()
    {
        vector<string> game_group_list;
        get_game_group_list(game_group_list);
        return game_group_list;
    }

    vector<string> get_item_vec(int game)
    {
        vector<string> item_list;
        get_item_list(item_list, game);
        return item_list;
    }

    vector<string> get_pokemon_vec(int game)
    {
        vector<string> pokemon_list;
        get_pokemon_list(pokemon_list, game);
        return pokemon_list;
    }

    vector<string> get_type_vec(int gen)
    {
        vector<string> type_list;
        get_type_list(type_list, gen);
        return type_list;
    }

    vector<string> get_ability_vec(int gen)
    {
        vector<string> ability_list;
        get_ability_list(ability_list, gen);
        return ability_list;
    }

    vector<string> get_nature_vec()
    {
        vector<string> nature_list;
        get_nature_list(nature_list);
        return nature_list;
    }
}
