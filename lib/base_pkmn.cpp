#include <iostream>
#include <string>
#include <pkmnsim/base_pkmn.hpp>

base_pkmn::base_pkmn(std::string name, int num, std::string spec, std::string t1, std::string t2, std::string a1, std::string a2, std::string a3,
                        double ht, double wt, double cm, double cf,
                        int bhp, int batk, int bdef, int bspd, int bsatk, int bsdef,
                        int ehp, int eatk, int edef, int espd, int esatk, int esdef)
{
    display_name = name;
    nat_pokedex_num = num;
    species = spec;
    type1 = t1;
    type2 = t2;
    ability1 = a1;
    ability2 = a2;
    ability3 = a3;
    height = ht;
    weight = wt;
    chance_male = cm;
    chance_female = cf;
    baseHP = bhp;
    baseATK = batk;
    baseDEF = bdef;
    baseSPD = bspd;
    baseSATK = bsatk;
    baseSDEF = bsdef;
    evHP = ehp;
    evATK = eatk;
    evDEF = edef;
    evSPD = espd;
    evSATK = esatk;
    evSDEF = esdef;
}

void base_pkmn::print()
{
    std::cout << display_name << " (#" << nat_pokedex_num << ")" << std::endl;
    std::cout << species << " Pokemon" << std::endl;
    std::cout << "Type: " << type1;
    if(type2 != "")
    {
        std::cout << "/" << type2 << std::endl;
    }
    else
    {
        std::cout << std::endl;
    }
    if((ability2 == "") && (ability3 == ""))
    {
        std::cout << "Ability: " << ability1 << std::endl;
    }
    else
    {
        std::cout << "Potential Abilities:" << std::endl;
        std::cout << " - " << ability1 << std::endl;
        std::cout << " - " << ability2 << std::endl;
        if(ability3 != "") std::cout << " - " << ability3 << std::endl;
    }
}

std::string base_pkmn::get_display_name() {return display_name;}
int base_pkmn::get_nat_pokedex_num() {return nat_pokedex_num;}
std::string base_pkmn::get_species() {return species;}
//get_types
double base_pkmn::get_height() {return height;}
double base_pkmn::get_weight() {return weight;}
double base_pkmn::get_chance_male() {return chance_male;}
double base_pkmn::get_chance_female() {return chance_female;}
//get_base_stats
//get_ev_yields
//get_move_list
