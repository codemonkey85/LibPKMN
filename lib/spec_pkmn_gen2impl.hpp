#ifndef INCLUDED_PKMNSIM_SPEC_PKMN_GEN2IMPL_HPP
#define INCLUDED_PKMNSIM_SPEC_PKMN_GEN2IMPL_HPP

#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <string>

namespace pkmnsim
{
    class PKMNSIM_API spec_pkmn_gen2impl: public spec_pkmn
    {
        public:
            spec_pkmn_gen2impl(base_pkmn::sptr, std::string, int);
            std::map<std::string, int> get_stats();
            std::map<std::string, int> get_IVs();
            std::map<std::string, int> get_EVs();
            std::string get_info();
            std::string get_info_verbose();
            char get_gender();

        private:
            int SATK, SDEF;
            int ivSATK, ivSDEF;
            int evSATK, evSDEF;
            char gender;
            std::string held_item;
            std::map<std::string, int> volatile_status_map;

            std::map<char, std::string> get_gender_map();
            void reset_volatile_status_map();
            int get_hp_from_iv_ev();
            int get_stat_from_iv_ev(std::string, int, int);
            char determine_gender();
    };
}

#endif /*INCLUDED_PKMNSIM_SPEC_PKMN_GEN2IMPL_HPP*/
