/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen4impl.hpp"
 
#include "conversions/trainer.hpp"

using namespace std;

namespace pkmnsim
{
    game_save_gen4impl::game_save_gen4impl(pokelib_sptr ptr): game_save()
    {
        pokelib_save = ptr;
        load();
    }
    
    void game_save_gen4impl::load()
    {
        game_trainer = conversions::import_trainer_from_pokelib(pokelib_save);
        game_id = game_trainer->get_game_id();
    }
    
    void game_save_gen4impl::save()
    {
        conversions::export_trainer_to_pokelib(game_trainer, pokelib_save);
    }
    
    trainer::sptr game_save_gen4impl::get_trainer() const {return game_trainer;}
} /* namespace pkmnsim */