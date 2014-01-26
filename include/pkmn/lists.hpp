/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_LISTS_HPP
#define INCLUDED_PKMN_LISTS_HPP

#include <iostream>
#include <string>
#include <vector>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/config.hpp>

namespace pkmn
{
    void PKMN_API get_game_list(std::vector<std::string> &game_vec);

    void PKMN_API get_game_group_list(std::vector<std::string> &game_group_vec);

    void PKMN_API get_item_list(std::vector<std::string> &item_vec, unsigned int game);

    void PKMN_API get_pokemon_list(std::vector<std::string> &pokemon_vec, unsigned int game);

    void PKMN_API get_type_list(std::vector<std::string> &type_vec, unsigned int gen);

    void PKMN_API get_ability_list(std::vector<std::string> &ability_vec, unsigned int gen);

    void PKMN_API get_nature_list(std::vector<std::string> &ability_vec);

    void PKMN_API get_pokemon_of_type(base_pokemon_vector &pkmn_vector, std::string type1, std::string type2, unsigned int gen, bool lax);

}

#endif /* INCLUDED_PKMN_LISTS_HPP */
