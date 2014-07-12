/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/filesystem.hpp>

#include "game_save_gen4impl.hpp"

#include "conversions/trainer.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen4impl::game_save_gen4impl(pokelib_sptr ptr,
                                           const std::string &filename): game_save_impl(filename)
    {
        _pokelib_save = ptr;
        load();
    }

    void game_save_gen4impl::load()
    {
        _trainer = conversions::import_gen4_trainer(_pokelib_save);
        _game_id = _trainer->get_game_id();
    }

    void game_save_gen4impl::save_as(const std::string &filename)
    {
        conversions::export_gen4_trainer(_trainer, _pokelib_save);
        //TODO: actual saving stuff
        _filepath = fs::path(filename);
    }
} /* namespace pkmn */
