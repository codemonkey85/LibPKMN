/*
 * Copyright (c) 2011 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/enums.hpp>

#include "game_save_gen1impl.hpp"
 
using namespace std;

namespace pkmnsim
{
    game_save_gen1impl::game_save_gen1impl(rpokesav_gen1_sptr sav)
    {
        _sav = sav;
        load();
    }
    
    void game_save_gen1impl::load()
    {
        _game_trainer = conversions::import_trainer_from_rpokesav_gen1(_sav);
        _game_id = Games::YELLOW;
    }

    void game_save_gen1impl::save() {};
    
    trainer::sptr game_save_gen1impl::get_trainer() const {return _game_trainer;}
} /* namespace pkmnsim */
