#ifndef SPEC_GEN2PKMN_HPP
#define SPEC_GEN2PKMN_HPP
#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/base_gen2pkmn.hpp>

class spec_gen2pkmn
{
    public:
        spec_gen2pkmn() {};
        spec_gen2pkmn(base_gen2pkmn, int, int);
        spec_gen2pkmn(base_gen2pkmn, std::string, int, char, std::string, int, int, int, int,
                  int, int, std::string, std::string, std::string, std::string);
        base_gen2pkmn get_base_gen2pkmn();
        void reset_volatile_status_map();
        void print();
        void print_verbose();
        
    private:
        base_gen2pkmn base;
        std::string nickname;
        int level;
        char gender; //M = male, F = female, N = nongendered
        std::string held_item;
        int HP, ATK, DEF, SPD, SATK, SDEF;
        int ivHP, ivATK, ivDEF, ivSPD, ivSATK, ivSDEF; //Individual values, determined in constructor
        int evHP, evATK, evDEF, evSPD, evSATK, evSDEF; //Effort values, earned via battles, 
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

        char determine_gender();
        int get_hp_from_iv_ev();
        int get_stat_from_iv_ev(int,int,int); //for others besides HP
};
#endif /*SPEC_GEN2PKMN_HPP*/
