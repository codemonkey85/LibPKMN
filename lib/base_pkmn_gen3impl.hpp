#ifndef INCLUDED_PKMNSIM_BASE_PKMN_GEN3IMPL_HPP
#define INCLUDED_PKMNSIM_BASE_PKMN_GEN3IMPL_HPP

#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <string>

namespace pkmnsim
{
    class base_pkmn_gen3impl: public base_pkmn
    {
        public:
            base_pkmn_gen3impl(std::string, int);
            std::string get_info();
            std::string get_info_verbose();
            std::map<std::string, int> get_base_stats();
            std::map<std::string, int> get_ev_yields();
            std::string * get_abilities();

        private:
            std::string ability1, ability2, ability3;
            int baseSATK, baseSDEF;
            int evHP, evATK, evDEF, evSATK, evSDEF, evSPD;
            double chance_male, chance_female;
    };
}

#endif /*INCLUDED_PKMNSIM_BASE_PKMN_GEN2IMPL_HPP*/
