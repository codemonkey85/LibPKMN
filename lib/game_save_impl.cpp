/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include <boost/format.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

#include <pkmnsim/enums.hpp>
#include <pkmnsim/game_save.hpp>
#include <pkmnsim/paths.hpp>

#include <pokehack/SaveParser.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

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
            PokeLib::Save save(filename.c_str());
            if(save.parseRawSave())
            {
                free(buffer);
                //return sptr(new game_save_gen4impl(save));
            }
            else
            {
                //Check to see if PokeLib accepts it as a proper Gen 4 save
                free(buffer);
                bw2sav_obj* sav = new bw2sav_obj;
                read(filename.c_str(), sav);
                if(savisbw2(sav))
                {
                    //return sptr(new game_save_gen5impl(sav));
                }
            }
        }
        else if(size > 0x40000)
        {
            PokeLib::Save save(filename.c_str());
            if(save.parseRawSave())
            {
                free(buffer);
                //return sptr(new game_save_gen4impl(save));
            }
        }
        else if(size >= 0x20000)
        {
            //Check for Gen 3 game by searching for game code
            int game_type;
            if(buffer[int(0xAC)] != 1) game_type = 0;
            else game_type = 1;

            SaveParser* parser = SaveParser::Instance();
            if(parser->load(filename.c_str(), game_type))
            {
                free(buffer);
                //return sptr(new game_save_gen3impl(parser));
            }
        }
        else
        {
            cerr << "This file is too small to be a proper save file." << endl;
            exit(EXIT_FAILURE);
        }
    }
}
