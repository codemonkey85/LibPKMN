/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen5impl.hpp"
 
using namespace std;

namespace pkmnsim
{
    game_save_gen5impl::game_save_gen5impl(pkmds_g5_sptr ptr): game_save()
    {
        sav = ptr;
    }
    
    void game_save_gen5impl::load() {};
    void game_save_gen5impl::save() {};
    void game_save_gen5impl::save_as(string filename) {};
    
    trainer::sptr game_save_gen5impl::get_trainer() const {return game_trainer;}
}
