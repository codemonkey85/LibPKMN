/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/types/pokemon_text.hpp>

BOOST_AUTO_TEST_CASE(multibyte_to_wide)
{
    std::string test_str("Test string");
    pkmn::pokemon_text test_pkmntext(test_str);
    std::wstring test_wstr(L"Test string");
    std::wstring wide_output(test_pkmntext.std_wstring());

    BOOST_CHECK(test_wstr == wide_output);
}

BOOST_AUTO_TEST_CASE(wide_to_multibyte)
{
    std::wstring test_wstr(L"Test string");
    pkmn::pokemon_text test_pkmntext(test_wstr);
    std::string test_str("Test string");
    std::string multibyte_output(test_pkmntext.std_string());
    
    BOOST_CHECK(test_str == multibyte_output);
}

BOOST_AUTO_TEST_CASE(multibyte_to_multibyte)
{
    std::string test_str("Test string");
    pkmn::pokemon_text test_pkmntext(test_str);
    BOOST_CHECK(test_str == test_pkmntext.std_string());
}

BOOST_AUTO_TEST_CASE(wide_to_wide)
{
    std::wstring test_wstr(L"Test string");
    pkmn::pokemon_text test_pkmntext(test_wstr);
    BOOST_CHECK(test_wstr == test_pkmntext.std_wstring());
}

BOOST_AUTO_TEST_CASE(comparison_operator)
{
    pkmn::pokemon_text test_pkmntext1("Test string");
    pkmn::pokemon_text test_pkmntext2("Test string");
    BOOST_CHECK(test_pkmntext1 == test_pkmntext2);
}