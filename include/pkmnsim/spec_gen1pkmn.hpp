#ifndef SPEC_GEN1PKMN_HPP
#define SPEC_GEN1PKMN_HPP
#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/base_gen1pkmn.hpp>

class spec_gen1pkmn
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
        std::string nickname;
        int level;
        int HP, ATK, DEF, SPD, SPCL;
        int ivHP, ivATK, ivDEF, ivSPD, ivSPCL; //Individual values, determined in constructor
        int evHP, evATK, evDEF, evSPD, evSPCL;
        std::string move1, move2, move3, move4; //Will possibly change moves to own class
        //TODO: Incorporate move PP
        int force_vals; //TODO: change to -1=random, 0-31=forced to that value

        std::string nonvolatile_status;
        /*
            Only one of the following:
            OK  = None
            BRN = Burn
            FRZ = Freeze
            PRZ = Paralysis
            PSN = Poison
            BP# = Bad Poison # (number of turns)
            SLP = Sleep
        */
        std::map<std::string, int> volatile_status_map; //Condition, number of turns left

        int get_hp_from_iv_ev();
        int get_stat_from_iv_ev(int,int,int); //for others besides HP
};
#endif /*SPEC_GEN1PKMN_HPP*/
