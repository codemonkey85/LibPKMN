/*
 * Copyright (c) 2011 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/filesystem.hpp>

#include <pkmn/enums.hpp>

#include "game_save_gen1impl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen1impl::game_save_gen1impl(rpokesav_gen1_sptr sav,
                                           const std::string &filename): game_save_impl(filename)
    {
        _sav = sav;
        load();
    }

    void game_save_gen1impl::load()
    {
        _trainer = conversions::import_gen1_trainer(_sav);
        _game_id = Games::YELLOW;
    }

    void game_save_gen1impl::save_as(const std::string &filename)
    {
        //TODO: actual saving stuff
        _filepath = fs::path(filename);
    };
} /* namespace pkmn */
