/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <ctime>

#include <boost/test/unit_test.hpp>

#include <pkmn/types/vla.hpp>

/*
 * LibPKMN includes a variable-length array class. This test will
 * pass if the array can be created with a variable size.
 */

BOOST_AUTO_TEST_CASE(vla_create_known_val)
{
    pkmn::vla<int> arr = pkmn::vla<int>(5);
    BOOST_CHECK(arr.size() == 5);
}

BOOST_AUTO_TEST_CASE(vla_create_variable_val)
{
    srand( time(NULL) );
    int arr_size = rand() % 100;
    pkmn::vla<int> arr = pkmn::vla<int>(arr_size);
    BOOST_CHECK(arr.size() == arr_size);
}
