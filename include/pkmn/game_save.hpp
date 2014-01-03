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

#include <pkmn/config.hpp>
#include <pkmn/trainer.hpp>

namespace pkmn
{
    class PKMNSIM_API game_save
    {
        public:

            typedef std::shared_ptr<game_save> sptr;
            static sptr make(std::string filename);

            //Class constructors (should never be called directly)
            game_save() {};
            virtual ~game_save() {};
            
            virtual void load() = 0;
            virtual void save() = 0;

            virtual trainer::sptr get_trainer() const = 0;
            
            //virtual std::vector<unsigned int> get_time_played() = 0;
            //virtual std::vector<unsigned int> get_last_play_time() = 0;

        protected:
        
            unsigned int _game_id;
            trainer::sptr _game_trainer;
            
            //unsigned int hours_played, minutes_played, seconds_played;
            //unsigned int year, month, day, hour, minute, second;
    };
}
#endif /* INCLUDED_PKMNSIM_GAME_SAVE_HPP */

