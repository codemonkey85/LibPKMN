/*
 * Copyright (c) 2011 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums.hpp>

#include "game_save_gen1impl.hpp"

using namespace std;

namespace pkmn
{
    game_save_gen1impl::game_save_gen1impl(rpokesav_gen1_sptr sav): game_save_impl()
    {
        _sav = sav;
        load();
    }

    void game_save_gen1impl::load()
    {
        _trainer = conversions::import_gen1_trainer(_sav);
        _game_id = Versions::YELLOW;
    }

    void game_save_gen1impl::save() {};
} /* namespace pkmn */
