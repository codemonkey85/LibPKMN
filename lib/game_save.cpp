/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/game_save.hpp>
#include <pkmnsim/paths.hpp>

#include "game_save_gen3impl.hpp"
#include "game_save_gen4impl.hpp"
#include "game_save_gen5impl.hpp"
#include "library_bridge.hpp"
#include "conversions/trainer.hpp"

using namespace std;

namespace pkmnsim
{
    game_save::sptr game_save::make(string filename)
    {
        //Read save file and get size
        FILE* save_file;
        save_file = fopen(filename.c_str(), "rb");
        fseek(save_file, 0, SEEK_END);
        int size = ftell(save_file);
        rewind(save_file);
        char* buffer = (char*)malloc(size);
        int result = fread(buffer, 1, size, save_file);
        fclose(save_file);

        //Once size is determined, determine whether or not save is valid
        if(size > 0x80000)
        {
            //Check to see if PokeLib accepts it as a proper Gen 4 save
            pokelib_sptr pokelib_save(new PokeLib::Save(filename.c_str()));
            if(pokelib_save->parseRawSave())
            {
                free(buffer);
                return sptr(new game_save_gen4impl(pokelib_save));
            }
            else
            {
                //Check to see if PKMDS accepts it as a proper Gen 5 save
                free(buffer);
                pkmds_g5_sptr sav = pkmds_g5_sptr(new bw2sav_obj);
                read(filename.c_str(), sav.get());
                if(savisbw2(sav.get())) return sptr(new game_save_gen5impl(sav));
            }
        }
        else if(size > 0x40000)
        {
            //Check to see if PokeLib accepts it as a proper Gen 4 save
            pokelib_sptr pokelib_save(new PokeLib::Save(filename.c_str()));
            if(pokelib_save->parseRawSave())
            {
                free(buffer);
                return sptr(new game_save_gen4impl(pokelib_save));
            }
        }
        else if(size >= 0x20000)
        {
            //Check for Gen 3 game by searching for game code
            int game_type = (buffer[int(0xAC)] == 1) ? 1 : 0;

            pokehack_sptr parser(new SaveParser);
            if(!parser->load(filename.c_str(), game_type))
            {
                return sptr(new game_save_gen3impl(parser, buffer));
            }
        }
        else
        {
            cerr << "This file is too small to be a proper save file." << endl;
            exit(EXIT_FAILURE);
        }
    }
} /* namespace pkmnsim */