#include <iostream>
#include <string>
#include <map>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <pkmnsim/pkmn_nature.hpp>

pkmn_nature::pkmn_nature(std::string nm, double atk, double def, double spd, double satk, double sdef)
{
    name = nm;
    ATKmod = atk;
    DEFmod = def;
    SPDmod = spd;
    SATKmod = satk;
    SDEFmod = sdef;
}

std::string pkmn_nature::get_name() {return name;}

double* pkmn_nature::get_mods()
{
    double *mod_list = new double[5];
    mod_list[0] = ATKmod;
    mod_list[1] = DEFmod;
    mod_list[2] = SPDmod;
    mod_list[3] = SATKmod;
    mod_list[4] = SDEFmod;
    return mod_list;
}

void pkmn_nature::print()
{
    std::cout << boost::format( "Nature: %s") % name << std::endl;
    std::cout << "Stat Mods:" << std::endl;
    std::cout << boost::format( " - Attack: %f") % ATKmod << std::endl;
    std::cout << boost::format( " - Defense: %f") % DEFmod << std::endl;
    std::cout << boost::format( " - Speed: %f") % SPDmod << std::endl;
    std::cout << boost::format( " - Special Attack: %f") % SATKmod << std::endl;
    std::cout << boost::format( " - Special Defense: %f") % SDEFmod << std::endl;
}
