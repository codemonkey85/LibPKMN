/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen4impl.hpp"
 
using namespace std;

namespace pkmnsim
{
    game_save_gen4impl::game_save_gen4impl(pokelib_sptr ptr): game_save()
    {
        pokelib_save = ptr;
    }
    
    void game_save_gen4impl::load() {};
    void game_save_gen4impl::save() {};
    void game_save_gen4impl::save_as(string filename) {};
    
    trainer::sptr game_save_gen4impl::get_trainer() const {return game_trainer;}
}
