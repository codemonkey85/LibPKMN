/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <boost/test/unit_test.hpp>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/move.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/types/dict.hpp>

#include "../lib/SQLiteCpp/SQLiteC++.h"

SQLite::Database db(pkmn::get_database_path().c_str());

/*
 * This test makes sure that LibPKMN's build successfully added
 * its entries to Veekun's database.
 */
BOOST_AUTO_TEST_CASE(has_database_additions_test)
{
    std::string query1_string("SELECT identifier FROM items WHERE id=1013");
    SQLite::Statement query1(db, query1_string.c_str());
    BOOST_CHECK(query1.executeStep());
    BOOST_CHECK(std::string((const char*)query1.getColumn(0)) == "gs-ball");

    std::string query2_string("SELECT name FROM stat_names WHERE stat_id=9");
    SQLite::Statement query2(db, query2_string.c_str());
    BOOST_CHECK(query2.executeStep());
    BOOST_CHECK(std::string((const char*)query2.getColumn(0)) == "Special");
    
    std::string query3_string("SELECT flavor_text FROM item_flavor_text WHERE item_id=1020");
    SQLite::Statement query3(db, query3_string.c_str());
    BOOST_CHECK(query3.executeStep());
    BOOST_CHECK(std::string((const char*)query3.getColumn(0)) == "Heart-print MAIL.");
    
    std::string query4_string("SELECT game_index FROM item_game_indices WHERE item_id=48 AND generation_id=2");
    SQLite::Statement query4(db, query4_string.c_str());
    BOOST_CHECK(query4.executeStep());
    BOOST_CHECK(int(query4.getColumn(0)) == 29);
    
    std::string query5_string("SELECT name FROM item_names WHERE item_id=1007");
    SQLite::Statement query5(db, query5_string.c_str());
    BOOST_CHECK(query5.executeStep());
    BOOST_CHECK(std::string((const char*)query5.getColumn(0)) == "Mint Berry");
    
    std::string query6_string("SELECT base_stat FROM pokemon_stats WHERE pokemon_id=92 AND stat_id=9");
    SQLite::Statement query6(db, query6_string.c_str());
    BOOST_CHECK(query6.executeStep());
    BOOST_CHECK(int(query6.getColumn(0)) == 100);
    
    std::string query7_string("SELECT exp_yield FROM old_exp_yields WHERE species_id=40");
    SQLite::Statement query7(db, query7_string.c_str());
    BOOST_CHECK(query7.executeStep());
    BOOST_CHECK(int(query7.getColumn(0)) == 109);

    std::string query8_string("SELECT gen4_accuracy FROM old_move_accuracies WHERE move_id=50");
    SQLite::Statement query8(db, query8_string.c_str());
    BOOST_CHECK(query8.executeStep());
    BOOST_CHECK(int(query8.getColumn(0)) == 80);

    std::string query9_string("SELECT gen2_power FROM old_move_powers WHERE move_id=91");
    SQLite::Statement query9(db, query9_string.c_str());
    BOOST_CHECK(query9.executeStep());
    BOOST_CHECK(int(query9.getColumn(0)) == 60);

    std::string query10_string("SELECT gen1_pp FROM old_move_pps WHERE move_id=202");
    SQLite::Statement query10(db, query10_string.c_str());
    BOOST_CHECK(query10.executeStep());
    BOOST_CHECK(int(query10.getColumn(0)) == 5);

    std::string query11_string("SELECT gen5_priority FROM old_move_priorities WHERE move_id=472");
    SQLite::Statement query11(db, query11_string.c_str());
    BOOST_CHECK(query11.executeStep());
    BOOST_CHECK(int(query11.getColumn(0)) == -7);

    std::string query12_string("SELECT name FROM old_move_names WHERE move_id=(SELECT move_id FROM move_names WHERE name='Feather Dance')");
    SQLite::Statement query12(db, query12_string.c_str());
    BOOST_CHECK(query12.executeStep());
    BOOST_CHECK(std::string((const char*)(query12.getColumn(0))) == "FeatherDance");
}

/*
 * This test makes sure these changes apply to the relevant base_pokemon implementations.
 */
BOOST_AUTO_TEST_CASE(base_pokemon_test)
{
    //Base stat changes
    pkmn::base_pokemon::sptr b_pkmn1 = pkmn::base_pokemon::make("Raichu", "Red");
    pkmn::base_pokemon::sptr b_pkmn2 = pkmn::base_pokemon::make("Raichu", "Gold");
    pkmn::base_pokemon::sptr b_pkmn3 = pkmn::base_pokemon::make("Raichu", "Diamond");
    pkmn::base_pokemon::sptr b_pkmn4 = pkmn::base_pokemon::make("Raichu", "X");
    
    BOOST_CHECK_EQUAL(b_pkmn1->get_base_stats()["Speed"], 100);
    BOOST_CHECK_EQUAL(b_pkmn2->get_base_stats()["Speed"], 100);
    BOOST_CHECK_EQUAL(b_pkmn3->get_base_stats()["Speed"], 100);
    BOOST_CHECK_EQUAL(b_pkmn4->get_base_stats()["Speed"], 110);

    //Experience yield changes
    pkmn::base_pokemon::sptr b_pkmn5 = pkmn::base_pokemon::make("Abra", "Red");
    pkmn::base_pokemon::sptr b_pkmn6 = pkmn::base_pokemon::make("Abra", "Gold");
    pkmn::base_pokemon::sptr b_pkmn7 = pkmn::base_pokemon::make("Abra", "Ruby");
    pkmn::base_pokemon::sptr b_pkmn8 = pkmn::base_pokemon::make("Abra", "Diamond");
    pkmn::base_pokemon::sptr b_pkmn9 = pkmn::base_pokemon::make("Abra", "White");

    BOOST_CHECK_EQUAL(b_pkmn5->get_exp_yield(), 73);
    BOOST_CHECK_EQUAL(b_pkmn6->get_exp_yield(), 73);
    BOOST_CHECK_EQUAL(b_pkmn7->get_exp_yield(), 73);
    BOOST_CHECK_EQUAL(b_pkmn8->get_exp_yield(), 75);
    BOOST_CHECK_EQUAL(b_pkmn9->get_exp_yield(), 62);
}

/*
 * This test makes sure these changes apply to the move class.
 */
BOOST_AUTO_TEST_CASE(move_test)
{
    //Accuracy changes
    pkmn::move::sptr move1 = pkmn::move::make("Low Kick", "Red");
    pkmn::move::sptr move2 = pkmn::move::make("Low Kick", "Gold");
    pkmn::move::sptr move3 = pkmn::move::make("Low Kick", "White");
    pkmn::move::sptr move4 = pkmn::move::make("Glare", "Diamond");
    pkmn::move::sptr move5 = pkmn::move::make("Glare", "White");
    pkmn::move::sptr move6 = pkmn::move::make("Glare", "X");

    BOOST_CHECK_EQUAL(move1->get_base_accuracy(), 0.9);
    BOOST_CHECK_EQUAL(move2->get_base_accuracy(), 1.0);
    BOOST_CHECK_EQUAL(move3->get_base_accuracy(), 1.0);
    BOOST_CHECK_EQUAL(move4->get_base_accuracy(), 0.75);
    BOOST_CHECK_EQUAL(move5->get_base_accuracy(), 0.9);
    BOOST_CHECK_EQUAL(move6->get_base_accuracy(), 1.0);

    //Power changes
    pkmn::move::sptr move7 = pkmn::move::make("Dig", "Red");
    pkmn::move::sptr move8 = pkmn::move::make("Dig", "Gold");
    pkmn::move::sptr move9 = pkmn::move::make("Dig", "Diamond");

    BOOST_CHECK_EQUAL(move1->get_base_power(), 50);
    BOOST_CHECK_EQUAL(move2->get_base_power(), 50);
    BOOST_CHECK_EQUAL(move3->get_base_power(), 0);
    BOOST_CHECK_EQUAL(move7->get_base_power(), 100);
    BOOST_CHECK_EQUAL(move8->get_base_power(), 60);
    BOOST_CHECK_EQUAL(move9->get_base_power(), 80);

    //PP changes
    pkmn::move::sptr move10 = pkmn::move::make("Giga Drain", "Ruby");
    pkmn::move::sptr move11 = pkmn::move::make("Giga Drain", "X");
    pkmn::move::sptr move12 = pkmn::move::make("Acid Armor", "Black 2");
    pkmn::move::sptr move13 = pkmn::move::make("Acid Armor", "X");

    BOOST_CHECK_EQUAL(move10->get_base_pp(), 5);
    BOOST_CHECK_EQUAL(move11->get_base_pp(), 10);
    BOOST_CHECK_EQUAL(move12->get_base_pp(), 40);
    BOOST_CHECK_EQUAL(move13->get_base_pp(), 20);

    //Priority changes
    pkmn::move::sptr move14 = pkmn::move::make("Roar", "Yellow");
    pkmn::move::sptr move15 = pkmn::move::make("Roar", "Silver");
    pkmn::move::sptr move16 = pkmn::move::make("Roar", "Emerald");
    pkmn::move::sptr move17 = pkmn::move::make("Roar", "White");
    pkmn::move::sptr move18 = pkmn::move::make("Roar", "Y");

    BOOST_CHECK_EQUAL(move14->get_priority(), 0);
    BOOST_CHECK_EQUAL(move15->get_priority(), -1);
    BOOST_CHECK_EQUAL(move16->get_priority(), -5);
    BOOST_CHECK_EQUAL(move17->get_priority(), -1);
    BOOST_CHECK_EQUAL(move18->get_priority(), -6);

    //Name changes
    pkmn::move::sptr move19 = pkmn::move::make(pkmn::Moves::ANCIENT_POWER, pkmn::Versions::LEAFGREEN);
    pkmn::move::sptr move20 = pkmn::move::make(pkmn::Moves::ANCIENT_POWER, pkmn::Versions::Y);
    pkmn::move::sptr move21 = pkmn::move::make(pkmn::Moves::SAND_ATTACK, pkmn::Versions::BLUE);
    pkmn::move::sptr move22 = pkmn::move::make(pkmn::Moves::SAND_ATTACK, pkmn::Versions::X);

    BOOST_CHECK_EQUAL(move19->get_name(), "AncientPower");
    BOOST_CHECK_EQUAL(move20->get_name(), "Ancient Power");
    BOOST_CHECK_EQUAL(move21->get_name(), "Sand-Attack");
    BOOST_CHECK_EQUAL(move22->get_name(), "Sand Attack");
}
