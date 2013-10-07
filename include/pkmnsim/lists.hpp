/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_LISTS_HPP
#define INCLUDED_PKMNSIM_LISTS_HPP

#include <iostream>
#include <string>
#include <vector>

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*
     * Returns a list of all Pokemon games.
     *
     * Parameters:
     *  - game_vec: Reference to a vector in which to place game list. 
     */
    void PKMNSIM_API get_game_list(std::vector<std::string>& game_vec);

    /*
     * Returns a list of all groups of Pokemon games.
     *
     * Parameters:
     *  - game_group_vec: Reference to a vector in which to place game group list.
     */
    void PKMNSIM_API get_game_group_list(std::vector<std::string>& game_group_vec);

    /*
     * Returns a list of all items in given game.
     *
     * Parameters:
     *  - item_vec: Reference to a vector in which to place item names.
     *  - game: Game from which to query item list
     */
    void PKMNSIM_API get_item_list(std::vector<std::string> &item_vec, int game);

    /*
     * Returns a list of all Pokemon in given game.
     *
     * Parameters:
     *  - pokemon_vec: Reference to a vector in which to place Pokemon names.
     *  - game: Game from which to query Pokemon list
     */
    void PKMNSIM_API get_pokemon_list(std::vector<std::string>& pokemon_vec, int game);

    /*
     * Returns a list of all types in given generation.
     *
     * Parameters:
     *  - type_vec: Reference to a vector in which to place type list.
     *  - gen: Generation from which to query types
     */
    void PKMNSIM_API get_type_list(std::vector<std::string>& type_vec, int gen);

    /*
     * Returns a list of all abilities in given generation.
     *
     * Paramters:
     *  - ability_vec: Reference to a vector in which to place ability names.
     *  - gen: Generation from which to query abilities
     */
    void PKMNSIM_API get_ability_list(std::vector<std::string>& ability_vec, int gen);

    /*
     * Returns a list of all natures.
     *
     * Paramters:
     *  - ability_vec: Reference to a vector in which to place nature names.
     */
    void PKMNSIM_API get_nature_list(std::vector<std::string>& ability_vec);

    void PKMNSIM_API get_pokemon_of_type(b_pkmn_vec_t &pkmn_vector, std::string type1, std::string type2, int gen, bool lax);

}

#endif /* INCLUDED_PKMNSIM_LISTS_HPP */
