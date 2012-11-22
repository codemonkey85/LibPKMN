#ifndef STAT_ANALYSIS_H
#define STAT_ANALYSIS_H

#include <pkmnsim/base_pkmn.hpp>

int return_min_possible_stat(base_pkmn, int, int);
int return_max_possible_stat(base_pkmn, int, int);
bool is_stat_possible(base_pkmn, int, int, int);

#endif /*STAT_ANALYSIS_H*/
