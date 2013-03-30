/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_SPEC_PKMN_GEN1IMPL_HPP
#define INCLUDED_PKMNSIM_SPEC_PKMN_GEN1IMPL_HPP

#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <string>

namespace pkmnsim
{
    class PKMNSIM_API spec_pkmn_gen1impl: public spec_pkmn
    {
        public:
            spec_pkmn_gen1impl(base_pkmn::sptr base, int lvl,
                               std::string m1, std::string m2,
                               std::string m3, std::string m4,
                               bool i);
            std::map<std::string, int> get_stats();
            std::map<std::string, int> get_IVs();
            std::map<std::string, int> get_EVs();
            std::string get_info();
            std::string get_info_verbose();

        private:
            int SPCL, ivSPCL, evSPCL;
            std::map<std::string, int> volatile_status_map;

            void reset_volatile_status_map();
            int get_hp_from_iv_ev();
            int get_stat_from_iv_ev(std::string, int, int);
    };
}

#endif /*INCLUDED_PKMNSIM_SPEC_PKMN_GEN1IMPL_HPP*/
