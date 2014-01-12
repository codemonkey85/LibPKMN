/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen3impl.hpp"
 
using namespace std;

namespace pkmn
{
    game_save_gen3impl::game_save_gen3impl(pokehack_sptr parser, char* buffer): game_save_impl()
    {
        _parser = parser;
        _data = buffer;
        load();
    }
    
    void game_save_gen3impl::load()
    {
        _trainer = conversions::import_trainer_from_pokehack(_parser, _data);
        _game_id = _trainer->get_game_id();
    }

    void game_save_gen3impl::save()
    {
        conversions::export_trainer_to_pokehack(_trainer, _parser, _data);
    }
} /* namespace pkmn */
