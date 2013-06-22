/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_SPEC_PKMN_GEN3IMPL_HPP
#define INCLUDED_SPEC_PKMN_GEN3IMPL_HPP

#include <string>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>

namespace pkmnsim
{
    class PKMNSIM_API spec_pkmn_gen345impl: public spec_pkmn
    {
        public:
            spec_pkmn_gen345impl(base_pkmn::sptr base, int lvl, int gen,
                               std::string m1, std::string m2, std::string m3,
                               std::string m4, bool i);
            dict<std::string, int> get_stats();
            dict<std::string, int> get_IVs();
            dict<std::string, int> get_EVs();
            std::string get_info();
            std::string get_info_verbose();
            int get_gender();
            pkmn_nature::sptr get_nature();
            std::string get_ability();

        private:
            int gender;
            std::string ability;
            std::string held_item;
            pkmn_nature::sptr nature;
            dict<std::string, int> volatile_status_map;

            void reset_volatile_status_map();
            int get_hp_from_iv_ev();
            int get_stat_from_iv_ev(std::string, int, int);
            int determine_gender();
            pkmn_nature::sptr determine_nature();
            std::string determine_ability();
    };
}

#endif /*INCLUDED_SPEC_PKMN_GEN3IMPL_HPP*/
