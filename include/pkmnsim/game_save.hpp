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
            static sptr make(std::string filename);

            //Class constructors (should never be called directly)
            game_save() {};
            virtual ~game_save() {};

            virtual trainer::sptr get_trainer() const = 0;
            
            virtual std::vector<unsigned int> get_time_played() = 0;
            virtual std::vector<unsigned int> get_last_play_time() = 0;

        protected:
            unsigned int game_id;
            unsigned char* raw_data;
            trainer::sptr game_trainer;
            
            unsigned int hours_played, minutes_played, seconds_played;
            unsigned int year, month, day, hour, minute, second;
    };
}
#endif /* INCLUDED_PKMNSIM_GAME_SAVE_HPP */
