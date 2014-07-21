/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/locale/encoding_utf.hpp>

#include <pkmn/enums.hpp>

#include "game_save_gen3impl.hpp"

#include "conversions/items.hpp"
#include "conversions/pokemon.hpp"
 
namespace fs = boost::filesystem;

namespace pkmn
{
    unsigned int _game_ids[] = {Versions::NONE, Versions::RUBY,
                                Versions::EMERALD, Versions::FIRERED};

    game_save_gen3impl::game_save_gen3impl(uint8_t* buffer,
                                           const std::string &filename): game_save_impl(filename)
    {
        _data = buffer;
        _libspec_save = gba_read_main_save(_data);

        _game_id = _game_ids[_libspec_save->type];

        load();
    }

    game_save_gen3impl::~game_save_gen3impl()
    {
        gba_free_save(_libspec_save);
    }
    
    void game_save_gen3impl::load()
    {
        _trainer = conversions::import_gen3_trainer(_libspec_save);
    }

    void game_save_gen3impl::save_as(const std::string &filename)
    {
        //TODO: actual saving stuff

        //Use size of original filepath to determine new filesize
        unsigned int size = fs::file_size(_filepath);
        std::ofstream ofile(filename.c_str());
        ofile.write((char*)_data, size);
        ofile.close();

        _filepath = fs::path(filename);
    }
} /* namespace pkmn */
