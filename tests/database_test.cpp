/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <pkmn/config.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/database/queries.hpp>

#include "../lib/SQLiteCpp/src/SQLiteC++.h"

/*
 * LibPKMN includes functions that act as an intermediary between the
 * user and the database, representing common functions. This test confirms
 * that the functions' queries have the expected results. This also tests
 * the accuracy of pkmn/enums.hpp.
 */

SQLite::Database db(pkmn::get_database_path().c_str());
std::string query_string;

BOOST_AUTO_TEST_CASE(test_database_lists_hpp)
{
    /*
     * pkmn::database::get_table_list
     */
    std::vector<std::string> function_vec;
    pkmn::database::get_table_list(function_vec);

    std::vector<std::string> manual_vec;
    SQLite::Statement query(db, "SELECT name FROM sqlite_master WHERE type='table'");
    while(query.executeStep())
    {
        std::string table_name = query.getColumn(0);
        manual_vec.push_back(table_name);
    }
    for(int i = 0; i < manual_vec.size(); i++) BOOST_CHECK_EQUAL(function_vec[i], manual_vec[i]);

    /*
     * pkmn::database::get_column_list
     */
    function_vec.clear();
    pkmn::database::get_column_list(function_vec, "item_game_indices");

    manual_vec.clear();
    SQLite::Statement query2(db, "PRAGMA table_info(item_game_indices)");
    while(query2.executeStep())
    {
        std::string column_name = query2.getColumn(1);
        manual_vec.push_back(column_name);
    }
    for(int i = 0; i < manual_vec.size(); i++) BOOST_CHECK_EQUAL(function_vec[i], manual_vec[i]);
}

BOOST_AUTO_TEST_CASE(ability_queries)
{
    /*
     * Getting ID's from names
     */
    unsigned int ability1_id = pkmn::database::get_ability_id("Torrent");
    unsigned int ability2_id = pkmn::database::get_ability_id("Fairy Aura");
    unsigned int ability3_id = pkmn::database::get_ability_id("Rough Skin");
    BOOST_CHECK_EQUAL(ability1_id, int(db.execAndGet("SELECT ability_id FROM ability_names WHERE name='Torrent'")));
    BOOST_CHECK_EQUAL(ability2_id, int(db.execAndGet("SELECT ability_id FROM ability_names WHERE name='Fairy Aura'")));
    BOOST_CHECK_EQUAL(ability3_id, int(db.execAndGet("SELECT ability_id FROM ability_names WHERE name='Rough Skin'")));

    /*
     * Getting names from ID's
     */
    std::string ability1_name = pkmn::database::get_ability_name(pkmn::Abilities::TORRENT);
    std::string ability2_name = pkmn::database::get_ability_name(pkmn::Abilities::FAIRY_AURA);
    std::string ability3_name = pkmn::database::get_ability_name(pkmn::Abilities::ROUGH_SKIN);
    BOOST_CHECK_EQUAL(ability1_name, (const char*)(db.execAndGet("SELECT name FROM ability_names WHERE local_language_id=9 AND ability_id=67")));
    BOOST_CHECK_EQUAL(ability2_name, (const char*)(db.execAndGet("SELECT name FROM ability_names WHERE local_language_id=9 AND ability_id=187")));
    BOOST_CHECK_EQUAL(ability3_name, (const char*)(db.execAndGet("SELECT name FROM ability_names WHERE local_language_id=9 AND ability_id=24")));
}

BOOST_AUTO_TEST_CASE(egg_group_queries)
{
    /*
     * Getting ID's from names
     */
    unsigned int egg_group1_id = pkmn::database::get_egg_group_id("Water 1");
    unsigned int egg_group2_id = pkmn::database::get_egg_group_id("Monster");
    unsigned int egg_group3_id = pkmn::database::get_egg_group_id("Bug");
    BOOST_CHECK_EQUAL(egg_group1_id, int(db.execAndGet("SELECT egg_group_id FROM egg_group_prose WHERE name='Water 1'")));
    BOOST_CHECK_EQUAL(egg_group2_id, int(db.execAndGet("SELECT egg_group_id FROM egg_group_prose WHERE name='Monster'")));
    BOOST_CHECK_EQUAL(egg_group3_id, int(db.execAndGet("SELECT egg_group_id FROM egg_group_prose WHERE name='Bug'")));

    /*
     * Getting names from ID's
     */
    std::string egg_group1_name = pkmn::database::get_egg_group_name(pkmn::Egg_Groups::WATER1);
    std::string egg_group2_name = pkmn::database::get_egg_group_name(pkmn::Egg_Groups::MONSTER);
    std::string egg_group3_name = pkmn::database::get_egg_group_name(pkmn::Egg_Groups::BUG);
    BOOST_CHECK_EQUAL(egg_group1_name, (const char*)(db.execAndGet("SELECT name FROM egg_group_prose WHERE local_language_id=9 AND egg_group_id=2")));
    BOOST_CHECK_EQUAL(egg_group2_name, (const char*)(db.execAndGet("SELECT name FROM egg_group_prose WHERE local_language_id=9 AND egg_group_id=1")));
    BOOST_CHECK_EQUAL(egg_group3_name, (const char*)(db.execAndGet("SELECT name FROM egg_group_prose WHERE local_language_id=9 AND egg_group_id=3")));
}

BOOST_AUTO_TEST_CASE(game_queries)
{
    /*
     * Getting ID's from names
     */
    unsigned int version1_id = pkmn::database::get_game_id("Red");
    unsigned int version2_id = pkmn::database::get_game_id("Ruby");
    unsigned int version3_id = pkmn::database::get_game_id("Black");
    BOOST_CHECK_EQUAL(version1_id, int(db.execAndGet("SELECT version_id FROM version_names WHERE name='Red'")));
    BOOST_CHECK_EQUAL(version2_id, int(db.execAndGet("SELECT version_id FROM version_names WHERE name='Ruby'")));
    BOOST_CHECK_EQUAL(version3_id, int(db.execAndGet("SELECT version_id FROM version_names WHERE name='Black'")));

    /*
     * Getting names from ID's
     */
    std::string version1_name = pkmn::database::get_game_name(pkmn::Games::RED);
    std::string version2_name = pkmn::database::get_game_name(pkmn::Games::RUBY);
    std::string version3_name = pkmn::database::get_game_name(pkmn::Games::BLACK);
    BOOST_CHECK_EQUAL(version1_name, (const char*)(db.execAndGet("SELECT name FROM version_names WHERE local_language_id=9 AND version_id=1")));
    BOOST_CHECK_EQUAL(version2_name, (const char*)(db.execAndGet("SELECT name FROM version_names WHERE local_language_id=9 AND version_id=7")));
    BOOST_CHECK_EQUAL(version3_name, (const char*)(db.execAndGet("SELECT name FROM version_names WHERE local_language_id=9 AND version_id=17")));
}

BOOST_AUTO_TEST_CASE(item_queries)
{
    /*
     * Getting ID's from game indices
     */
    unsigned int item1_id = pkmn::database::get_item_id(pkmn::Items::LURE_BALL, pkmn::Games::SOUL_SILVER);
    unsigned int item2_id = pkmn::database::get_item_id(pkmn::Items::HYPER_POTION, pkmn::Games::RED);
    unsigned int item3_id = pkmn::database::get_item_id(pkmn::Items::MYSTERYBERRY, pkmn::Games::CRYSTAL);
    BOOST_CHECK_EQUAL(item1_id, int(db.execAndGet("SELECT item_id FROM item_game_indices WHERE game_index=494 AND generation_id=4")));

    /*
     * Getting ID's from names
     */
    item1_id = pkmn::database::get_item_id("Master Ball");
    item2_id = pkmn::database::get_item_id("Silph Scope");
    item3_id = pkmn::database::get_item_id("Pecha Berry");
    BOOST_CHECK_EQUAL(item1_id, int(db.execAndGet("SELECT item_id FROM item_names WHERE name='Master Ball'")));
    BOOST_CHECK_EQUAL(item2_id, int(db.execAndGet("SELECT item_id FROM item_names WHERE name='Silph Scope'")));
    BOOST_CHECK_EQUAL(item3_id, int(db.execAndGet("SELECT item_id FROM item_names WHERE name='Pecha Berry'")));

    /*
     * Getting names from ID's
     */
    std::string item1_name = pkmn::database::get_item_name(pkmn::Items::MASTER_BALL);
    std::string item2_name = pkmn::database::get_item_name(pkmn::Items::SILPH_SCOPE);
    std::string item3_name = pkmn::database::get_item_name(pkmn::Items::PECHA_BERRY);
    BOOST_CHECK_EQUAL(item1_name, (const char*)(db.execAndGet("SELECT name FROM item_names WHERE local_language_id=9 AND item_id=1")));
    BOOST_CHECK_EQUAL(item2_name, (const char*)(db.execAndGet("SELECT name FROM item_names WHERE local_language_id=9 AND item_id=548")));
    BOOST_CHECK_EQUAL(item3_name, (const char*)(db.execAndGet("SELECT name FROM item_names WHERE local_language_id=9 AND item_id=128")));
}
