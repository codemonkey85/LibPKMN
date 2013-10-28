/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP
#define INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP

#include "team_pokemon_impl.hpp"

namespace pkmnsim
{
    class team_pokemon_gen1impl: public team_pokemon_impl
    {
        public:

            team_pokemon_gen1impl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
                                  unsigned int move1, unsigned int move2,
                                  unsigned int move3, unsigned int move4);

            std::string get_info() const;
            std::string get_info_verbose() const;
            
            unsigned int get_ability() const;
            unsigned int get_gender() const;
            unsigned int get_nature() const;
            bool is_shiny() const;
            
            dict<unsigned int, unsigned int> get_stats() const;
            dict<unsigned int, unsigned int> get_EVs() const;
            dict<unsigned int, unsigned int> get_IVs() const;
            
            void set_EV(unsigned int EV, unsigned int val);
            void set_IV(unsigned int IV, unsigned int val);
            
            std::string get_form_name() const;
            unsigned int get_form_id() const;
            
            void set_form(unsigned int form);
            void set_form(std::string form);

        private:
        
            unsigned int get_hp() const;
            unsigned int get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP */