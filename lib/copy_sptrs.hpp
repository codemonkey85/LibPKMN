/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_COPY_SPTRS_HPP
#define INCLUDED_COPY_SPTRS_HPP

#include <pkmn/bag.hpp>
#include <pkmn/base_pokemon.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/item.hpp>
#include <pkmn/move.hpp>
#include <pkmn/pocket.hpp>

namespace pkmn
{
    bag::sptr copy_bag(bag::sptr in);

    base_pokemon::sptr copy_base_pokemon(base_pokemon::sptr in);

    //game_save::sptr copy_game_save(game_save::sptr in);

    item::sptr copy_item(item::sptr in);

    move::sptr copy_move(move::sptr in);

    pocket::sptr copy_pocket(pocket::sptr in);

    team_pokemon::sptr copy_team_pokemon(team_pokemon::sptr in);

    trainer::sptr copy_trainer(trainer::sptr in);
}

#endif /* INCLUDED_COPY_SPTRS_HPP */
