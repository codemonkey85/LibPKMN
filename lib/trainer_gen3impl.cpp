/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include "sqlitecpp/SQLiteCPP.h"
#include "trainer_gen3impl.hpp"

using namespace std;

namespace pkmnsim
{
    trainer_gen3impl::trainer_gen3impl(SQLite::Database *import_db): trainer()
    {
        parser = SaveParser::Instance();

        SQLite::Database pkmnsim_db(get_database_path().c_str());

        SQLite::Statement trainer_info_query(*import_db, "SELECT * FROM trainer_info");
        trainer_info_query.executeStep();

        from_game = trainer_info_query.getColumn(1);
        trainer_name = trainer_info_query.getColumnStr(2);
        trainer_id = trainer_info_query.getColumnStr(3);
        int party_size = trainer_info_query.getColumn(4);
        money = trainer_info_query.getColumn(5);

        party.clear();

        for(unsigned int i = 0; i < party_size; i++)
        {
            //Grab values from export database necessary to create spec_pkmn
            SQLite::Statement party_query(*import_db, str(boost::format("SELECT * FROM party WHERE id=%d")
                                                                        % i).c_str()
                                         );
            party_query.executeStep();

            int pkmn_id = party_query.getColumn(1);
            int species_id = party_query.getColumn(2);
            string nickname = party_query.getColumn(3);
            int level = party_query.getColumn(4);
            int item_held_id = party_query.getColumn(5);
            int move1_id = party_query.getColumn(6);
            int move2_id = party_query.getColumn(7);
            int move3_id = party_query.getColumn(8);
            int move4_id = party_query.getColumn(9);
            int HP = party_query.getColumn(10);
            int ATK = party_query.getColumn(11);
            int DEF = party_query.getColumn(12);
            int SATK = party_query.getColumn(13);
            int SDEF = party_query.getColumn(14);
            int SPD = party_query.getColumn(15);
            int evHP = party_query.getColumn(16);
            int evATK = party_query.getColumn(17);
            int evDEF = party_query.getColumn(18);
            int evSATK = party_query.getColumn(19);
            int evSDEF = party_query.getColumn(20);
            int evSPD = party_query.getColumn(21);
            int ivHP = party_query.getColumn(22);
            int ivATK = party_query.getColumn(23);
            int ivDEF = party_query.getColumn(24);
            int ivSATK = party_query.getColumn(25);
            int ivSDEF = party_query.getColumn(26);
            int ivSPD = party_query.getColumn(27);
            string nature_name = party_query.getColumn(28);

            string identifier, item_held, move1, move2, move3, move4;
            identifier = pkmnsim_db.execAndGetStr(str(boost::format(
                                                      "SELECT name FROM pokemon_species_names WHERE pokemon_species_id=%d AND local_language_id=9")
                                                      % species_id).c_str(), "name"
                                                 );
            if(item_held_id == -1) item_held = "None";
            else
            {
                item_held = pkmnsim_db.execAndGetStr(str(boost::format(
                                                         "SELECT name FROM item_names WHERE item_id=%d")
                                                         % item_held_id).c_str(), "name"
                                                    );
            }
            move1 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                 "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                 % move1_id).c_str(), "name"
                                            );
            if(move2_id == -1) move2 = "None";
            else
            {
                move2 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                     "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                     % move2_id).c_str(), "name"
                                                );
            }
            if(move3_id == -1) move3 = "None";
            else
            {
                move3 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                     "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                     % move3_id).c_str(), "name"
                                                );
            }
            if(move4_id == -1) move4 = "None";
            else
            {
                move4 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                     "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                     % move4_id).c_str(), "name"
                                                );
            }

            spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 3, level, move1, move2, move3, move4, true);

            //Manually set other values
            s_pkmn->nickname = nickname;
            s_pkmn->HP = HP;
            s_pkmn->ATK = ATK;
            s_pkmn->DEF = DEF;
            s_pkmn->SATK = SATK;
            s_pkmn->SDEF = SDEF;
            s_pkmn->SPD = SPD;
            s_pkmn->evHP = evHP;
            s_pkmn->evATK = evATK;
            s_pkmn->evDEF = evDEF;
            s_pkmn->evSATK = evSATK;
            s_pkmn->evSDEF = evSDEF;
            s_pkmn->evSPD = evSPD;
            s_pkmn->ivHP = ivHP;
            s_pkmn->ivATK = ivATK;
            s_pkmn->ivDEF = ivDEF;
            s_pkmn->ivSATK = ivSATK;
            s_pkmn->ivSDEF = ivSDEF;
            s_pkmn->ivSPD = ivSPD;
            s_pkmn->nature = pkmn_nature::make(nature_name);

            party.push_back(s_pkmn);
        }
    }

    trainer_gen3impl::trainer_gen3impl(string filename, int game): trainer()
    {
        parser = SaveParser::Instance();

        from_game = game;
        int game_type;

        switch(game)
        {
            case Games::RUBY:
            case Games::SAPPHIRE:
            case Games::EMERALD:
                game_type = 0;
                break;

            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                game_type = 1;
                break;

            default:
                cerr << "Only Ruby, Sapphire, Emerald, Fire Red, and Leaf Green currently supported." << endl;
                exit(EXIT_FAILURE);
        }

        int err = parser->load(filename.c_str(), game_type);
        if(err)
        {
            cerr << "Error loading game save file." << endl;
            exit(EXIT_FAILURE);
        }

        money = 0; //Placeholder until money is implemented in Pokehack
        trainer_name = parser->get_text(parser->pokemon[0]->otname, 7);
        short* trainer_id_short = (short*)&(parser->pokemon[0]->otid);
        trainer_id = boost::lexical_cast<string>(trainer_id_short[0]);

        party.clear();
        for(int i = 0; i < 6; i++)
            if(string(pokemon_species[parser->pokemon_growth[i]->species]) != "No pokemon")
                party.push_back(convert_to_spec_pkmn(parser->pokemon[i],
                                                     parser->pokemon_attacks[i],
                                                     parser->pokemon_effort[i],
                                                     parser->pokemon_misc[i],
                                                     parser->pokemon_growth[i]));
    }


    spec_pkmn::sptr trainer_gen3impl::convert_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                             pokemon_attacks_t* pkmn_a_t,
                                                             pokemon_effort_t* pkmn_e_t,
                                                             pokemon_misc_t* pkmn_m_t,
                                                             pokemon_growth_t* pkmn_g_t)
    {
        string identifier, move1, move2, move3, move4;
        int level;

        identifier = pokemon_species[pkmn_g_t->species];
        level = b_pkmn_t->level;

        move1 = database::to_database_format(attacks[pkmn_a_t->atk1]);
        if(string(attacks[pkmn_a_t->atk2]) == "No attack") move2 = "None";
        else move2 = database::to_database_format(attacks[pkmn_a_t->atk2]);
        if(string(attacks[pkmn_a_t->atk3]) == "No attack") move3 = "None";
        else move3 = database::to_database_format(attacks[pkmn_a_t->atk3]);
        if(string(attacks[pkmn_a_t->atk4]) == "No attack") move4 = "None";
        else move4 = database::to_database_format(attacks[pkmn_a_t->atk4]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 3, level,
                                                 move1, move2, move3, move4,
                                                 true);

        s_pkmn->nickname = parser->get_text(b_pkmn_t->name, 10);
        s_pkmn->held_item = items[pkmn_g_t->held];

        string nature_from_pokehack = natures[b_pkmn_t->personality % 25];
        vector<string> nature_halves;
        boost::split(nature_halves, nature_from_pokehack, boost::is_any_of(" "));
        s_pkmn->nature = pkmn_nature::make(nature_halves[0]);

        //Gender
        int gender_from_pokehack = b_pkmn_t->personality % 256;
        double chance_male = s_pkmn->get_base_pkmn()->get_chance_male();
        double chance_female = s_pkmn->get_base_pkmn()->get_chance_female();
        if(chance_male == 0.0 and chance_female == 0.0) gender = Genders::GENDERLESS;
        else if(chance_male == 0.0) gender = Genders::FEMALE;
        else if(chance_female == 0.0) gender = Genders::MALE;
        else
        {
            if(gender_from_pokehack > int(floor(255*(1-chance_male)))) gender = Genders::MALE;
            else gender = Genders::FEMALE;
        }
        

        s_pkmn->HP = b_pkmn_t->maxHP;
        s_pkmn->ATK = b_pkmn_t->attack;
        s_pkmn->DEF = b_pkmn_t->defense;
        s_pkmn->SATK = b_pkmn_t->spatk;
        s_pkmn->SDEF = b_pkmn_t->spdef;
        s_pkmn->SPD = b_pkmn_t->speed;

        s_pkmn->ivHP = pkmn_m_t->IVs.hp;
        s_pkmn->ivATK = pkmn_m_t->IVs.atk;
        s_pkmn->ivDEF = pkmn_m_t->IVs.def;
        s_pkmn->ivSATK = pkmn_m_t->IVs.spatk;
        s_pkmn->ivSDEF = pkmn_m_t->IVs.spdef;
        s_pkmn->ivSPD = pkmn_m_t->IVs.spd;

        s_pkmn->evHP = pkmn_e_t->hp;
        s_pkmn->evATK = pkmn_e_t->attack;
        s_pkmn->evDEF = pkmn_e_t->defense;
        s_pkmn->evSATK = pkmn_e_t->spatk;
        s_pkmn->evSDEF = pkmn_e_t->spdef;
        s_pkmn->evSPD = pkmn_e_t->speed;

        return s_pkmn;
    }

}
