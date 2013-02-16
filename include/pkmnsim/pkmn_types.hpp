#ifndef PKMN_TYPES_HPP
#define PKMN_TYPES_HPP

#include <ciso646>
#include <string>
#include <vector>

namespace pkmnsim
{
    double get_type_damage_mod(std::string, std::string, bool);
    std::vector<std::string> get_type_names(int);
}

#endif /*PKMN_TYPES_HPP*/
