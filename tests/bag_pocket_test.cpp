/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/bag.hpp>
#include <pkmn/pocket.hpp>

/*
 * When instantiating a bag, it automatically populates with the appropriate
 * pockets. This test confirms that this is done correctly for each version group.
 */

BOOST_AUTO_TEST_CASE(gen1_bag)
{
    pkmn::bag::sptr item_bag = pkmn::bag::make("Blue");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 20);
    BOOST_CHECK(pocket_dict.size() == 1);
}

BOOST_AUTO_TEST_CASE(gen2_bag)
{
    pkmn::bag::sptr item_bag = pkmn::bag::make("Silver");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 20);
    BOOST_CHECK(pocket_dict.has_key("Balls"));
    BOOST_CHECK(pocket_dict["Balls"]->get_size() == 12);
    BOOST_CHECK(pocket_dict.has_key("KeyItems"));
    BOOST_CHECK(pocket_dict["KeyItems"]->get_size() == 26);
    BOOST_CHECK(pocket_dict.has_key("TM/HM"));
    BOOST_CHECK(pocket_dict["TM/HM"]->get_size() == 99);
    BOOST_CHECK(pocket_dict.size() == 4);
}

BOOST_AUTO_TEST_CASE(RS_bag)
{
    pkmn::bag::sptr item_bag = pkmn::bag::make("Sapphire");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 20);
    BOOST_CHECK(pocket_dict.has_key("Poke Balls"));
    BOOST_CHECK(pocket_dict["Poke Balls"]->get_size() == 16);
    BOOST_CHECK(pocket_dict.has_key("TMs and HMs"));
    BOOST_CHECK(pocket_dict["TMs and HMs"]->get_size() == 64);
    BOOST_CHECK(pocket_dict.has_key("Berries"));
    BOOST_CHECK(pocket_dict["Berries"]->get_size() == 46);
    BOOST_CHECK(pocket_dict.has_key("Key Items"));
    BOOST_CHECK(pocket_dict["Key Items"]->get_size() == 20);
    BOOST_CHECK(pocket_dict.size() == 5);
}

BOOST_AUTO_TEST_CASE(emerald_bag)
{
    pkmn::bag::sptr item_bag = pkmn::bag::make("Emerald");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 20);
    BOOST_CHECK(pocket_dict.has_key("Poke Balls"));
    BOOST_CHECK(pocket_dict["Poke Balls"]->get_size() == 16);
    BOOST_CHECK(pocket_dict.has_key("TMs and HMs"));
    BOOST_CHECK(pocket_dict["TMs and HMs"]->get_size() == 64);
    BOOST_CHECK(pocket_dict.has_key("Berries"));
    BOOST_CHECK(pocket_dict["Berries"]->get_size() == 46);
    BOOST_CHECK(pocket_dict.has_key("Key Items"));
    BOOST_CHECK(pocket_dict["Key Items"]->get_size() == 30);
    BOOST_CHECK(pocket_dict.size() == 5);
}

BOOST_AUTO_TEST_CASE(FRLG_bag)
{
    pkmn::bag::sptr item_bag = pkmn::bag::make("Leaf Green");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 42);
    BOOST_CHECK(pocket_dict.has_key("Key Items"));
    BOOST_CHECK(pocket_dict["Key Items"]->get_size() == 30);
    BOOST_CHECK(pocket_dict.has_key("Poke Balls"));
    BOOST_CHECK(pocket_dict["Poke Balls"]->get_size() == 13);
    BOOST_CHECK(pocket_dict.has_key("TM Case"));
    BOOST_CHECK(pocket_dict["TM Case"]->get_size() == 58);
    BOOST_CHECK(pocket_dict.has_key("Berry Pouch"));
    BOOST_CHECK(pocket_dict["Berry Pouch"]->get_size() == 43);
    BOOST_CHECK(pocket_dict.size() == 5);
}

BOOST_AUTO_TEST_CASE(DPP_bag)
{
    pkmn::bag::sptr item_bag = pkmn::bag::make("Pearl");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 165);
    BOOST_CHECK(pocket_dict.has_key("Medicine"));
    BOOST_CHECK(pocket_dict["Medicine"]->get_size() == 40);
    BOOST_CHECK(pocket_dict.has_key("Poke Balls"));
    BOOST_CHECK(pocket_dict["Poke Balls"]->get_size() == 15);
    BOOST_CHECK(pocket_dict.has_key("TMs and HMs"));
    BOOST_CHECK(pocket_dict["TMs and HMs"]->get_size() == 100);
    BOOST_CHECK(pocket_dict.has_key("Berries"));
    BOOST_CHECK(pocket_dict["Berries"]->get_size() == 64);
    BOOST_CHECK(pocket_dict.has_key("Mail"));
    BOOST_CHECK(pocket_dict["Mail"]->get_size() == 12);
    BOOST_CHECK(pocket_dict.has_key("Battle Items"));
    BOOST_CHECK(pocket_dict["Battle Items"]->get_size() == 30);
    BOOST_CHECK(pocket_dict.has_key("Key Items"));
    BOOST_CHECK(pocket_dict["Key Items"]->get_size() == 50);
    BOOST_CHECK(pocket_dict.size() == 8);
}

BOOST_AUTO_TEST_CASE(HGSS_bag)
{   
    pkmn::bag::sptr item_bag = pkmn::bag::make("Heart Gold");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 165);
    BOOST_CHECK(pocket_dict.has_key("Medicine"));
    BOOST_CHECK(pocket_dict["Medicine"]->get_size() == 40);
    BOOST_CHECK(pocket_dict.has_key("Poke Balls"));
    BOOST_CHECK(pocket_dict["Poke Balls"]->get_size() == 24);
    BOOST_CHECK(pocket_dict.has_key("TMs and HMs"));
    BOOST_CHECK(pocket_dict["TMs and HMs"]->get_size() == 100);
    BOOST_CHECK(pocket_dict.has_key("Berries"));
    BOOST_CHECK(pocket_dict["Berries"]->get_size() == 64);
    BOOST_CHECK(pocket_dict.has_key("Mail"));
    BOOST_CHECK(pocket_dict["Mail"]->get_size() == 12);
    BOOST_CHECK(pocket_dict.has_key("Battle Items"));
    BOOST_CHECK(pocket_dict["Battle Items"]->get_size() == 30);
    BOOST_CHECK(pocket_dict.has_key("Key Items"));
    BOOST_CHECK(pocket_dict["Key Items"]->get_size() == 43);
    BOOST_CHECK(pocket_dict.size() == 8);
}

BOOST_AUTO_TEST_CASE(gen56_bags)
{
    pkmn::bag::sptr item_bag = pkmn::bag::make("Black 2");
    pkmn::dict<std::string, pkmn::pocket::sptr> pocket_dict = item_bag->get_pockets();
    BOOST_CHECK(pocket_dict.has_key("Items"));
    BOOST_CHECK(pocket_dict["Items"]->get_size() == 310);
    BOOST_CHECK(pocket_dict.has_key("Medicine"));
    BOOST_CHECK(pocket_dict["Medicine"]->get_size() == 48);
    BOOST_CHECK(pocket_dict.has_key("TMs and HMs"));
    BOOST_CHECK(pocket_dict["TMs and HMs"]->get_size() == 109);
    BOOST_CHECK(pocket_dict.has_key("Berries"));
    BOOST_CHECK(pocket_dict["Berries"]->get_size() == 64);
    BOOST_CHECK(pocket_dict.has_key("Key Items"));
    BOOST_CHECK(pocket_dict["Key Items"]->get_size() == 83);
    BOOST_CHECK(pocket_dict.size() == 5);
}