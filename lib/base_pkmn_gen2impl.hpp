#ifndef INCLUDED_PKMNSIM_BASE_PKMN_GEN2IMPL_HPP
#define INCLUDED_PKMNSIM_BASE_PKMN_GEN2IMPL_HPP

#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <string>

namespace pkmnsim
{
    class PKMNSIM_API base_pkmn_gen2impl: public base_pkmn
    {
        public:
            base_pkmn_gen2impl(std::string);
            std::string get_info();
            std::string get_info_verbose();
            std::map<std::string, int> get_base_stats();
            std::map<std::string, int> get_ev_yields();

        private:
            int baseSATK, baseSDEF;
            int evHP, evATK, evDEF, evSATK, evSDEF, evSPD;
            double chance_male, chance_female;
    };
}

#endif /*INCLUDED_PKMNSIM_BASE_PKMN_GEN2IMPL_HPP*/
