/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_IMPL_HPP
#define INCLUDED_GAME_SAVE_IMPL_HPP

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include <pkmn/game_save.hpp>

#include "conversions/trainer.hpp"

namespace pkmn
{
    class game_save_impl: public game_save, boost::noncopyable
    {
        public:
        
            game_save_impl() {};
            game_save_impl(uint8_t* buffer, std::string orig_filename);

            void save() const;
            
            unsigned int get_game_id() const;
            
            trainer::sptr get_trainer() const;
            
            bag::sptr get_trainer_bag() const;
            
            void get_trainer_party(pokemon_team_t& party) const;
            void set_trainer_party(pokemon_team_t& party);
            
            pokemon_text get_trainer_name() const;
            void set_trainer_name(pokemon_text trainer_name);
            
        protected:

            uint8_t* _data;
            boost::filesystem::path _filepath;

            unsigned int _game_id;
            trainer::sptr _trainer;
    };
}
#endif /* INCLUDED_GAME_SAVE_IMPL_HPP */
