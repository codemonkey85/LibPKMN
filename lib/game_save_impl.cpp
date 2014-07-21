/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/paths.hpp>

#include "game_save_impl.hpp"
#include "game_save_gen1impl.hpp"
#include "game_save_gen3impl.hpp"
#include "game_save_gen4impl.hpp"
#include "game_save_gen5impl.hpp"
#include "library_bridge.hpp"
#include "conversions/trainer.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save::sptr game_save::make(const std::string &filename)
    {
        uint32_t size = fs::file_size(fs::path(filename));

        if(size >= 0x80000)
        {
            //Check to see if PokeLib-NC accepts this as a proper Gen IV save
            pokelib_sptr pokelib_save(new PokeLib::Save(filename.c_str()));
            if(pokelib_save->parseRawSave())
            {
                return sptr(new game_save_gen4impl(pokelib_save, filename));
            }
            else
            {
                //Check to see if PKMDS accepts this as a proper Gen V save
                pkmds_g5_sptr sav = pkmds_g5_sptr(new bw2sav_obj);
                ::read(filename.c_str(), sav.get());
                if(::savisbw2(sav.get())) return sptr(new game_save_gen5impl(sav, filename));
            }
        }
        else if(size >= 0x40000)
        {
            //Check to see if PokeLib-NC accepts this as a proper Gen IV save
            pokelib_sptr pokelib_save(new PokeLib::Save(filename.c_str()));
            if(pokelib_save->parseRawSave())
            {
                return sptr(new game_save_gen4impl(pokelib_save, filename));
            }
        }
        else if(size >= 0x20000)
        {
            //Check to see if LibSPEC accepts this as a proper Gen III save
            std::ifstream ifile(filename.c_str(), std::ios::binary);
            uint8_t* buffer = (uint8_t*)malloc(size);
            ifile.read((char*)buffer, size);

            if(gba_is_gba_save(buffer)) return sptr(new game_save_gen3impl(buffer, filename));
        }
        else if(size >= (2 << 14))
        {
            //Check to see if Retro Pokesav accepts this as a proper Gen I save
            rpokesav_gen1_sptr g1_sav(new rpokesav::gen1_save(filename));
            if(g1_sav->check()) return sptr(new game_save_gen1impl(g1_sav, filename));
        }

        throw std::runtime_error("This is not a valid save file.");
    }

    game_save_impl::game_save_impl(const std::string &filename)
    {
        _filepath = fs::path(filename);
    }

    void game_save_impl::save()
    {
        save_as(_filepath.string());
    }

    unsigned int game_save_impl::get_game_id() const {return _game_id;}

    trainer::sptr game_save_impl::get_trainer() const {return _trainer;}

    bag::sptr game_save_impl::get_trainer_bag() const {return _trainer->get_bag();}

    void game_save_impl::get_trainer_party(pokemon_team_t& party) {_trainer->get_party(party);}

    void game_save_impl::set_trainer_party(pokemon_team_t& party) {_trainer->set_party(party);}

    pokemon_text game_save_impl::get_trainer_name() const {return _trainer->get_name();}

    void game_save_impl::set_trainer_name(pokemon_text trainer_name) {_trainer->set_name(trainer_name);}

} /* namespace pkmn */
