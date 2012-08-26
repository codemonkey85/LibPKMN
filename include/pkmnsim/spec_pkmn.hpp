#include <iostream>
#include <string>
#include <pkmnsim/base_pkmn.hpp>

class spec_pkmn
{
    public:
        spec_pkmn() {};
        get_base_pkmn();
        
    private:
        base_pkmn base;
        int level;
        char gender; //M = male, F = female, N = neutral
        std::string nature;
        int ATK, DEF, SPD, SATK, SDEF;
        int ivATK, ivDEF, ivSPD, ivSATK, ivSDEF; //Individual values, determined in beginning
        std::string move1, move2, move3, move4; //Will possible change moves to own class

        std::string nonvolatile_status;
        /*
            Only one of the following:
            BRN = Burn
            FRZ = Freeze
            PRZ = Paralysis
            PSN = Poison
            BP# = Bad Poison # (number of turns)
            SLP = Sleep
        */
        std::map<std::string, int> volative_status_map; //Condition, number of turns left
        
        int force_vals; //0 = none, -1 = min, 1 = max 
}
