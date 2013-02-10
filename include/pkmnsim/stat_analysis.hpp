#ifndef STAT_ANALYSIS_HPP
#define STAT_ANALYSIS_HPP

#include <pkmnsim/base_pkmn.hpp>
#include <string>

int return_min_possible_stat(pkmnsim::base_pkmn::sptr, std::string, int);
int return_max_possible_stat(pkmnsim::base_pkmn::sptr, std::string, int);
bool is_stat_possible(pkmnsim::base_pkmn::sptr, int, std::string, int);

#endif /*STAT_ANALYSIS_HPP*/
