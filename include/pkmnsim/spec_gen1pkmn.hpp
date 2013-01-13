#ifndef SPEC_GEN1PKMN_HPP
#define SPEC_GEN1PKMN_HPP

#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/base_gen1pkmn.hpp>
#include <pkmnsim/spec_pkmn.hpp>

class spec_gen1pkmn: public spec_pkmn
{
    public:
        spec_gen1pkmn() {};
        spec_gen1pkmn(base_gen1pkmn, int, int);
        spec_gen1pkmn(base_gen1pkmn, std::string, int, int, int, int, int, int,
                  std::string, std::string, std::string, std::string);
        base_gen1pkmn get_base_gen1pkmn();
        void reset_volatile_status_map();
        void print();
        void print_verbose();
        
    private:
        base_gen1pkmn base;
        int SPCL, ivSPCL, evSPCL;
        int get_hp_from_iv_ev();
        int get_stat_from_iv_ev(int,int,int); //for others besides HP
};

#endif /*SPEC_GEN1PKMN_HPP*/
