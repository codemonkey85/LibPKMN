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

namespace pkmnsim
{
    class PKMNSIM_API base_pkmn_gen345impl: public base_pkmn
    {
        public:
            base_pkmn_gen345impl(std::string, int);
            std::string get_info(void);
            std::string get_info_verbose(void);
            dict<std::string, int> get_base_stats(void);
            dict<std::string, int> get_ev_yields(void);
            double get_chance_male();
            double get_chance_female();
            dict<int, std::string> get_abilities(void);
            bool has_gender_differences(void);
            std::string get_sprite_path(bool is_male, bool is_shiny);

        private:
            std::string ability1, ability2, ability3;
            int baseSATK, baseSDEF;
            int evHP, evATK, evDEF, evSATK, evSDEF, evSPD;
            double chance_male, chance_female;
    };
}

#endif /* INCLUDED_BASE_PKMN_GEN3IMPL_HPP */
