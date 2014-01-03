/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/types/dict.hpp>

/*
 * Part of LibPKMNsim's constraints is that Boost cannot be publicly exposed,
 * so an alternative to BOOST_FOREACH is required. MSVC has a foreach method,
 * but it is not portable, so a custom #define is necessary. If this doesn't
 * work, then the Boostless pkmnsim::dict class will not work.
 */

BOOST_AUTO_TEST_CASE(dict_has_key)
{
    const pkmnsim::dict<int, int> b_dict = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    BOOST_CHECK(b_dict.has_key(0));
    BOOST_CHECK(not b_dict.has_key(3));
}

BOOST_AUTO_TEST_CASE(dict_keys_vals)
{
    const pkmnsim::dict<int, int> b_dict = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    std::vector<int> keys = b_dict.keys();
    BOOST_CHECK(std::find(keys.begin(), keys.end(), 0) != keys.end());
    BOOST_CHECK(std::find(keys.begin(), keys.end(), 3) == keys.end());

    std::vector<int> vals = b_dict.vals();
    BOOST_CHECK(std::find(vals.begin(), vals.end(), 57) != vals.end());
    BOOST_CHECK(std::find(vals.begin(), vals.end(), 1) == vals.end());
}

BOOST_AUTO_TEST_CASE(dict_get)
{
    const pkmnsim::dict<int, int> b_dict = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    BOOST_CHECK(b_dict.get(0) == -1);
    BOOST_CHECK(b_dict.get(0,5) == -1);
    BOOST_CHECK(b_dict.get(1,5) == 5);
}
