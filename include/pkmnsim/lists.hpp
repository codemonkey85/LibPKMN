/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LISTS_HPP
#define INCLUDED_LISTS_HPP

#include <iostream>
#include <string>
#include <vector>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*
     * Lists
     */
    void PKMNSIM_API get_game_list(std::vector<std::string>& game_vec);
    void PKMNSIM_API get_game_group_list(std::vector<std::string>& game_vec);
    //void PKMNSIM_API get_pokemon_list(std::vector<std::string>& pkmn_vec);
    void PKMNSIM_API get_type_list(std::vector<std::string>& type_vec, int gen);

    std::vector<std::string> PKMNSIM_API get_game_vec();
    std::vector<std::string> PKMNSIM_API get_game_group_vec();
    std::vector<std::string> PKMNSIM_API get_type_vec(int gen);
}

#endif /* INCLUDED_LISTS_HPP */
