/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen3impl.hpp"
 
using namespace std;

namespace pkmnsim
{
    game_save_gen3impl::game_save_gen3impl(pokehack_sptr ptr, char* buffer): game_save()
    {
        parser = ptr;
        data = buffer;
        load();
    }
    
    void game_save_gen3impl::load()
    {
        game_trainer = conversions::import_trainer_from_pokehack(parser, data);
        game_id = game_trainer->get_game_id();
    }

    void game_save_gen3impl::save()
    {
        conversions::export_trainer_to_pokehack(game_trainer, parser, data);
    }
    
    trainer::sptr game_save_gen3impl::get_trainer() const {return game_trainer;}
}
