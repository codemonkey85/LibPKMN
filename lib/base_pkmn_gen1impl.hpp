/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_BASE_PKMN_GEN1IMPL_HPP
#define INCLUDED_PKMNSIM_BASE_PKMN_GEN1IMPL_HPP

#include <map>
#include "sqlitecpp/SQLiteCPP.h"
#include <pkmnsim/base_pkmn.hpp>
#include <string>

namespace pkmnsim
{
    class PKMNSIM_API base_pkmn_gen1impl: public base_pkmn
    {
        public:
            base_pkmn_gen1impl(std::string identifier, SQLite::Database *db);
            std::string get_info();
            std::string get_info_verbose();
            std::map<std::string, int> get_base_stats();
            std::map<std::string, int> get_ev_yields();

        private:
            int baseSPCL;
    };
}

#endif /*INCLUDED_PKMNSIM_BASE_PKMN_GEN1IMPL_HPP*/
