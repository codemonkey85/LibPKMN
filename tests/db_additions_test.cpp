/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <boost/test/unit_test.hpp>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/types/dict.hpp>

#include "../lib/SQLiteCpp/src/SQLiteC++.h"

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

    std::string query2_string("SELECT name FROM stat_names WHERE id=9");
    SQLite::Statement query2(db, query2_string.c_str());
    BOOST_CHECK(query2.executeStep());
    BOOST_CHECK(std::string((const char*)query2.getColumn(0)) == "Special");
    
    std::string query3_string("SELECT flavor_text FROM item_flavor_text WHERE item=1020");
    SQLite::Statement query3(db, query3_string.c_str());
    BOOST_CHECK(query3.executeStep());
    BOOST_CHECK(std::string((const char*)query3.getColumn(0)) == "Heart-print MAIL.");
    
    std::string query4_string("SELECT game_index FROM item_id=48 AND generation_id=2");
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
}

/*
 * This test makes sure these changes apply to the relevant base_pokemon implementations.
 */
BOOST_AUTO_TEST_CASE(base_pokemon_test)
{
    //Base stat changes
    pkmn::base_pokemon::sptr b_pkmn1 = pkmn::base_pokemon::make("Raichu","Red");
    pkmn::base_pokemon::sptr b_pkmn2 = pkmn::base_pokemon::make("Raichu","Gold");
    pkmn::base_pokemon::sptr b_pkmn3 = pkmn::base_pokemon::make("Raichu","Diamond");
    pkmn::base_pokemon::sptr b_pkmn4 = pkmn::base_pokemon::make("Raichu","X");
    
    BOOST_CHECK_EQUAL(b_pkmn1->get_base_stats()["Speed"], 100);
    BOOST_CHECK_EQUAL(b_pkmn2->get_base_stats()["Speed"], 100);
    BOOST_CHECK_EQUAL(b_pkmn3->get_base_stats()["Speed"], 100);
    BOOST_CHECK_EQUAL(b_pkmn4->get_base_stats()["Speed"], 110);

    //Experience yield changes
    pkmn::base_pokemon::sptr b_pkmn5 = pkmn::base_pokemon::make("Abra","Red");
    pkmn::base_pokemon::sptr b_pkmn6 = pkmn::base_pokemon::make("Abra","Gold");
    pkmn::base_pokemon::sptr b_pkmn7 = pkmn::base_pokemon::make("Abra","Ruby");
    pkmn::base_pokemon::sptr b_pkmn8 = pkmn::base_pokemon::make("Abra","Diamond");
    pkmn::base_pokemon::sptr b_pkmn9 = pkmn::base_pokemon::make("Abra","White");

    BOOST_CHECK_EQUAL(b_pkmn5->get_exp_yield(), 73);
    BOOST_CHECK_EQUAL(b_pkmn6->get_exp_yield(), 73);
    BOOST_CHECK_EQUAL(b_pkmn7->get_exp_yield(), 73);
    BOOST_CHECK_EQUAL(b_pkmn8->get_exp_yield(), 75);
    BOOST_CHECK_EQUAL(b_pkmn9->get_exp_yield(), 62);
}