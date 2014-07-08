/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen5impl.hpp"

#include "conversions/trainer.hpp"

using namespace std;

namespace pkmn
{
    game_save_gen5impl::game_save_gen5impl(pkmds_g5_sptr ptr): game_save_impl()
    {
        _sav = ptr;
        load();
    }

    void game_save_gen5impl::load()
    {
        _trainer = conversions::import_gen5_trainer(_sav);
        _game_id = _trainer->get_game_id();
    }

    void game_save_gen5impl::save()
    {
        conversions::export_gen5_trainer(_trainer, _sav);
    }
} /* namespace pkmn */
