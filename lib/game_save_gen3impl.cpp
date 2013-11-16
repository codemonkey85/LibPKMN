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
    game_save_gen3impl::game_save_gen3impl(pokehack_sptr ptr): game_save()
    {
        parser = ptr;
    }
    
    void game_save_gen3impl::load() {};
    void game_save_gen3impl::save() {};
    
    trainer::sptr game_save_gen3impl::get_trainer() const {return game_trainer;}
}
