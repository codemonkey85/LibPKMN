/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_PKMN_GEN3IMPL_HPP
#define INCLUDED_BASE_PKMN_GEN3IMPL_HPP

#include <string>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/dict.hpp>

#include "sqlitecpp/SQLiteCPP.h"

namespace pkmnsim
{
    class PKMNSIM_API base_pkmn_gen3impl: public base_pkmn
    {
        public:
            base_pkmn_gen3impl(std::string, int, SQLite::Database *db, bool query_moves);
            std::string get_info(void);
            std::string get_info_verbose(void);
            dict<std::string, int> get_base_stats(void);
            dict<std::string, int> get_ev_yields(void);
            double get_chance_male();
            double get_chance_female();
            dict<int, std::string> get_abilities(void);

        private:
            std::string ability1, ability2, ability3;
            int baseSATK, baseSDEF;
            int evHP, evATK, evDEF, evSATK, evSDEF, evSPD;
            double chance_male, chance_female;
    };
}

#endif /* INCLUDED_BASE_PKMN_GEN3IMPL_HPP */
