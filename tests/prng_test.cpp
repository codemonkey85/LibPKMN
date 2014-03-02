/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <pkmn/types/prng.hpp>

BOOST_AUTO_TEST_CASE(check_singleton_addresses)
{
    pkmn::prng::sptr gen1_instance = pkmn::prng::get(1);
    pkmn::prng::sptr gen2_instance = pkmn::prng::get(2);
    pkmn::prng::sptr gen3_instance1 = pkmn::prng::get(3);
    pkmn::prng::sptr gen3_instance2 = pkmn::prng::get(3);
    pkmn::prng::sptr gen4_instance1 = pkmn::prng::get(4);
    pkmn::prng::sptr gen4_instance2 = pkmn::prng::get(4);
    pkmn::prng::sptr gen5_instance1 = pkmn::prng::get(5);
    pkmn::prng::sptr gen5_instance2 = pkmn::prng::get(5);
    pkmn::prng::sptr gen6_instance1 = pkmn::prng::get(6);
    pkmn::prng::sptr gen6_instance2 = pkmn::prng::get(6);
    
    BOOST_CHECK(gen1_instance.get() == gen2_instance.get());
    BOOST_CHECK(gen2_instance.get() == gen3_instance1.get());
    BOOST_CHECK(gen1_instance.get() == gen3_instance1.get());
    BOOST_CHECK(gen3_instance1.get() == gen3_instance2.get());
    BOOST_CHECK(gen4_instance1.get() == gen4_instance2.get());
    BOOST_CHECK(gen5_instance1.get() == gen5_instance2.get());
    BOOST_CHECK(gen6_instance1.get() == gen6_instance2.get());
    BOOST_CHECK(gen3_instance1.get() != gen4_instance1.get());
    BOOST_CHECK(gen4_instance1.get() != gen5_instance1.get());
    BOOST_CHECK(gen5_instance1.get() != gen6_instance1.get());
}