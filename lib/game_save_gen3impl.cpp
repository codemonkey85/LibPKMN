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
        _save = gba_read_main_save(_data);
        __trainer = gba_get_trainer(_save);
        _party = gba_get_party(_save);
        _pc = gba_get_pc(_save);

        _game_id = _game_ids[_save->type];

        load();
    }

    game_save_gen3impl::~game_save_gen3impl()
    {
        gba_free_save(_save);
    }
    
    void game_save_gen3impl::load()
    {
        uint16_t name_arr[7];
        gba_text_to_ucs2((char16_t*)name_arr, (char8_t*)__trainer->name, 7);
        pokemon_text trainer_name(boost::locale::conv::utf_to_utf<wchar_t>(name_arr));

        _trainer = trainer::make(_game_id, trainer_name, __trainer->gender);

        for(size_t i = 0; i < 6; i++)
        {
            _trainer->set_pokemon(i+1, conversions::import_gen3_pokemon(&(_party->pokemon[i]),
                                                                        _save->type));
        }
        conversions::import_gen3_items(_trainer->get_bag(), _save, _save->type);
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
