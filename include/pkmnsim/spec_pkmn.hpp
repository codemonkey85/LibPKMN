#include <iostream>
#include <string>
#include <pkmnsim/base_pkmn.hpp>

class spec_pkmn
{
    public:
        spec_pkmn() {};
        spec_pkmn(base_pkmn, int, int);
        spec_pkmn(base_pkmn, std::string, int, char, std::string, std::string, int, int, int, int,
                  int, int, std::string, std::string, std::string, std::string);
        //base_pkmn get_base_pkmn();
        
    private:
        base_pkmn base;
        std::string nickname;
        int level;
        char gender; //M = male, F = female, N = neutral
        std::string nature;
        std::string held_item;
        int HP, ATK, DEF, SPD, SATK, SDEF;
        int ivHP, ivATK, ivDEF, ivSPD, ivSATK, ivSDEF; //Individual values, determined in beginning
        std::string move1, move2, move3, move4; //Will possible change moves to own class
        int force_vals; //0 = none, -1 = min, 1 = max 

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
        std::string determine_nature();
        int get_hp_from_iv();
        int get_stat_from_iv(int,int); //for others besides HP
        int get_iv_from_hp();
        int get_iv_from_stat(int,int); //for others besides HP
        std::map<std::string, int> init_volatile_status_map(); //Can also reset
};
