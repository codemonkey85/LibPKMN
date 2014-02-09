/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/types/dict.hpp>

/*
 * Part of LibPKMN's constraints is that Boost cannot be publicly exposed,
 * so an alternative to BOOST_FOREACH is required. MSVC has a foreach method,
 * but it is not portable, so a custom #define is necessary. If this doesn't
 * work, then the Boostless pkmn::dict class will not work.
 */
BOOST_AUTO_TEST_CASE(pkmn_dict_has_key)
{
    const pkmn::dict<int, int> b_dict = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    BOOST_CHECK(b_dict.has_key(0));
    BOOST_CHECK(not b_dict.has_key(3));
}

/*
 * Test getting and setting with pkmn::dict's [] operator
 */
BOOST_AUTO_TEST_CASE(pkmn_dict_operator)
{
    pkmn::dict<int, int> b_dict = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    int test_var = b_dict[0];
    BOOST_CHECK_EQUAL(test_var, -1);

    b_dict[4] = 10;
    BOOST_CHECK_EQUAL(b_dict[4], 10);

    b_dict[10] = 7;
    BOOST_CHECK(b_dict.has_key(10));
    BOOST_CHECK_EQUAL(b_dict.at(10,11), 7);
}

/*
 * Testing both pkmn::dict::at() functions
 */
BOOST_AUTO_TEST_CASE(pkmn_dict_at)
{
    const pkmn::dict<int, int> b_dict = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    BOOST_CHECK_EQUAL(b_dict.at(2), 5);
    BOOST_CHECK_EQUAL(b_dict.at(3,10), 10);
}

/*
 * Testing whether pkmn::dict::swap() swaps
 */
BOOST_AUTO_TEST_CASE(pkmn_dict_swap)
{
    pkmn::dict<int, int> b_dict = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    b_dict.swap(0,4);
    BOOST_CHECK_EQUAL(b_dict[0], 57);
    BOOST_CHECK_EQUAL(b_dict[4], -1);
}