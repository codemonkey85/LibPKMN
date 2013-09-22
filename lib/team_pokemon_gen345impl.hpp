/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_GEN345IMPL_HPP
#define INCLUDED_TEAM_POKEMON_GEN345IMPL_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>

#include "team_pokemon_impl.hpp"

namespace pkmnsim
{
    class PKMNSIM_API team_pokemon_gen345impl: public team_pokemon_impl
    {
        public:

            team_pokemon_gen345impl(unsigned int identifier, unsigned int game, unsigned int level,
                                    unsigned int move1, unsigned int move2,
                                    unsigned int move3, unsigned int move4);

            std::string get_info() const;
            std::string get_info_verbose() const;
            
            unsigned int get_volatile_status(int status) const;
            void set_volatile_status(int status, int val);
            
            unsigned int get_ability() const;
            unsigned int get_gender() const;
            unsigned int get_nature() const;
            bool is_shiny() const;
            
            dict<unsigned int, unsigned int> get_stats() const;
            dict<unsigned int, unsigned int> get_IVs() const;
            dict<unsigned int, unsigned int> get_EVs() const;
            
            void set_EV(unsigned int EV, unsigned int val);
            void set_IV(unsigned int IV, unsigned int val);
            void set_move(move::sptr, unsigned int pos);
            
            unsigned int get_nature() const;
            unsigned int get_held_item() const;
            void set_held_item(int item);

        private:
        
            unsigned int get_hp_from_ev_iv() const;
            unsigned int get_stat_from_ev_iv(unsigned int stat, unsigned int EV, unsigned int IV) const;
            
            void reset_volatile_status_map();
    };
}

#endif /* INCLUDED_TEAM_POKEMON_GEN345IMPL_HPP */