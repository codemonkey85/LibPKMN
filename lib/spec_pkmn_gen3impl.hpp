#ifndef INCLUDED_PKMNSIM_SPEC_PKMN_GEN3IMPL_HPP
#define INCLUDED_PKMNSIM_SPEC_PKMN_GEN3IMPL_HPP

#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <string>

namespace pkmnsim
{
    class spec_pkmn_gen3impl: public spec_pkmn
    {
        public:
            spec_pkmn_gen3impl(base_pkmn::sptr, std::string, int);
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
            std::string ability;
            std::string held_item;
            pkmn_nature nature;
            std::map<std::string, int> volatile_status_map;

            std::map<char, std::string> get_gender_map();
            void reset_volatile_status_map();
            int get_hp_from_iv_ev();
            int get_stat_from_iv_ev(std::string, int, int);
            char determine_gender();
            pkmnsim::pkmn_nature determine_nature();
            std::string determine_ability();
    };
}

#endif /*INCLUDED_PKMNSIM_SPEC_PKMN_GEN3IMPL_HPP*/
