/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_GAME_SAVE_HPP
#define INCLUDED_PKMNSIM_GAME_SAVE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <string>
#include <vector>

#include <pkmnsim/config.hpp>
#include <pkmnsim/trainer.hpp>

namespace pkmnsim
{
    class PKMNSIM_API game_save
    {
        public:

            typedef std::shared_ptr<trainer> sptr;

            //Class constructors (should never be called directly)
            game_save(void);

            static sptr make(std::string filename);

        protected:
            int game_id;
            unsigned char* raw_data;
            trainer::sptr game_trainer;
    };
}

#endif /* INCLUDED_PKMNSIM_GAME_SAVE_HPP */
