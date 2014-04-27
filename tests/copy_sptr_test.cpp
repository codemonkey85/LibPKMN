/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/test/unit_test.hpp>

#include <pkmn/team_pokemon.hpp>

/*
 * This test's LibPKMN's internal functionality for copying shared pointers,
 * which comes into place in custom copy constructors and assignment operators.
 */

BOOST_AUTO_TEST_CASE(base_pokemon_from_team_pokemon)
{
    pkmn::team_pokemon::sptr t_pkmn = pkmn::team_pokemon::make("Arceus", "X", 70,
                                                               "None", "None",
                                                               "None", "None");

    pkmn::base_pokemon::sptr b_pkmn1 = t_pkmn->get_base_pokemon(true); //Copy
    pkmn::base_pokemon::sptr b_pkmn2 = t_pkmn->get_base_pokemon(false); //Pointer to t_pkmn's base_pokemon

    b_pkmn1->set_form("Grass");
    b_pkmn2->set_form("Fire");

    BOOST_CHECK_EQUAL(t_pkmn->get_form_id(), b_pkmn2->get_form_id());
    BOOST_CHECK(t_pkmn->get_form_id() != b_pkmn1->get_form_id());
}
