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
#include <pkmn/team_pokemon.hpp>
#include <pkmn/trainer.hpp>

namespace pkmn
{
    bag::sptr copy_bag(bag::sptr original);
    base_pokemon::sptr copy_base_pokemon(base_pokemon::sptr original);
    game_save::sptr copy_game_save(game_save::sptr original);
    item::sptr copy_item(item::sptr original);
    move::sptr copy_move(move::sptr original);
    pocket::sptr copy_pocket(pocket::sptr original);
    team_pokemon::sptr copy_team_pokemon(team_pokemon::sptr original);
    trainer::sptr copy_trainer(trainer::sptr original);
}

#endif /* INCLUDED_COPY_SPTRS_HPP */