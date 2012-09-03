#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <pkmnsim/base_pkmn.hpp>

base_pkmn::base_pkmn(std::string name, int num, std::string spec, std::string t1, std::string t2, std::string a1, std::string a2, std::string a3,
                        double ht, double wt, double cm, double cf,
                        int bhp, int batk, int bdef, int bspd, int bsatk, int bsdef,
                        int ehp, int eatk, int edef, int espd, int esatk, int esdef, int exp)
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
    exp_yield = exp;
}

void base_pkmn::print()
{
    std::cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << std::endl;
    if(type2 != "") std::cout << boost::format("Type: %s") % type1 << std::endl;
    else std::cout << boost::format("Type: %s/%s") % type1 % type2 << std::endl;
    std::cout << boost::format("Stats: %d,%d,%d,%d,%d,%d") %
                               baseHP % baseATK % baseDEF % baseSPD % baseSATK % baseSDEF << std::endl;
}

void base_pkmn::print_verbose()
{
    std::cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << std::endl;
    std::cout << boost::format("%s Pokemon") % species << std::endl;

    if(type2.empty()) std::cout << boost::format("Type: %s") % type1 << std::endl;
    else std::cout << boost::format("Type: %s/%s") % type1 % type2 << std::endl;

    if((ability2.empty()) && (ability3.empty())) std::cout << boost::format("Ability: %s") % ability1 << std::endl;
    else
    {
        std::cout << "Potential Abilities:" << std::endl;
        std::cout << boost::format(" - %s") % ability1 << std::endl;
        if(!ability2.empty()) std::cout << boost::format(" - %s") % ability2 << std::endl;
        if(!ability3.empty()) std::cout << boost::format(" - %s") % ability3 << std::endl;
    }

    std::cout << boost::format("%d m, %d kg") % height % weight << std::endl;
    if((chance_male + chance_female) == 0.0) std::cout << "Genderless" << std::endl;
    else std::cout << boost::format("%2.1f%s Male, %2.1f%s Female") % (chance_male*100) % "%" % (chance_female*100) % "%" << std::endl;

    std::cout << "Base Stats:" << std::endl;
    std::cout << boost::format(" - HP: %d") % baseHP << std::endl;
    std::cout << boost::format(" - Attack: %d") % baseATK << std::endl;
    std::cout << boost::format(" - Defense: %d") % baseDEF << std::endl;
    std::cout << boost::format(" - Speed: %d") % baseSPD << std::endl;
    std::cout << boost::format(" - Special Attack: %d") % baseSATK << std::endl;
    std::cout << boost::format(" - Special Defense: %d") % baseSDEF << std::endl;

    std::cout << "Effort Value Yields:" << std::endl;
    std::cout << boost::format(" - HP: %d") % evHP << std::endl;
    std::cout << boost::format(" - Attack: %d") % evATK << std::endl;
    std::cout << boost::format(" - Defense: %d") % evDEF << std::endl;
    std::cout << boost::format(" - Speed: %d") % evSPD << std::endl;
    std::cout << boost::format(" - Special Attack: %d") % evSATK << std::endl;
    std::cout << boost::format(" - Special Defense: %d") % evSDEF << std::endl;

    std::cout << boost::format("Experience Yield: %d") % exp_yield << std::endl;
}

std::string base_pkmn::get_display_name() {return display_name;}
int base_pkmn::get_nat_pokedex_num() {return nat_pokedex_num;}
std::string base_pkmn::get_species() {return species;}
//get_types

std::string* base_pkmn::get_types()
{
    std::string *types = new std::string[2];
    types[0] = type1;
    types[1] = type2;
    return types;
}

std::string* base_pkmn::get_abilities()
{
    std::string *abilities = new std::string[3];
    abilities[0] = ability1;
    abilities[1] = ability2;
    abilities[2] = ability3;
    return abilities;
}

double base_pkmn::get_height() {return height;}
double base_pkmn::get_weight() {return weight;}
double base_pkmn::get_chance_male() {return chance_male;}
double base_pkmn::get_chance_female() {return chance_female;}

int* base_pkmn::get_base_stats()
{
    int *stats = new int[6];

    stats[0] = baseHP;
    stats[1] = baseATK;
    stats[2] = baseDEF;
    stats[3] = baseSPD;
    stats[4] = baseSATK;
    stats[5] = baseSDEF;

    return stats;
}

int* base_pkmn::get_ev_yields()
{
    int *yields = new int[7];

    yields[0] = evHP;
    yields[1] = evATK;
    yields[2] = evDEF;
    yields[3] = evSPD;
    yields[4] = evSATK;
    yields[5] = evSDEF;
    yields[6] = exp_yield;

    return yields;
}
//get_move_list
