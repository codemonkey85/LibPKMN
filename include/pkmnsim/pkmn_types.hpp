#ifndef PKMN_TYPES_HPP
#define PKMN_TYPES_HPP

#include <pkmnsim/config.hpp>
#include <string>
#include <vector>

namespace pkmnsim
{
    double PKMNSIM_API get_type_damage_mod(std::string, std::string, bool);
    std::vector<std::string> PKMNSIM_API get_type_names(int);
}

#endif /*PKMN_TYPES_HPP*/
