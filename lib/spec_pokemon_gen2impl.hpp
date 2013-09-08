/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_SPEC_PKMN_GEN2IMPL_HPP
#define INCLUDED_SPEC_PKMN_GEN2IMPL_HPP

#include <string>

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/spec_pokemon.hpp>

namespace pkmnsim
{
    class spec_pokemon_gen2impl: public spec_pokemon
    {
        public:
            spec_pokemon_gen2impl(base_pokemon::sptr base, int lvl, int g,
                               int m1, int m2, int m3, int m4);
            dict<std::string, int> get_stats();
            dict<std::string, int> get_IVs();
            void set_IV(std::string IVname, int val);
            dict<std::string, int> get_EVs();
            void set_EV(std::string EVname, int val);
            std::string get_held_item();
            void set_held_item(std::string name);
            std::string get_info();
            std::string get_info_verbose();
            int get_gender();
            bool is_shiny();
            void set_form(int form);
            void set_form(std::string form);

        private:
            dict<std::string, int> volatile_status_map;

            void reset_volatile_status_map();
            int get_hp_from_iv_ev();
            int get_stat_from_iv_ev(std::string, int, int);
            int determine_gender();
    };
}

#endif /*INCLUDED_SPEC_PKMN_GEN2IMPL_HPP*/
