#include <math.h>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/stat_analysis.hpp>

int return_min_possible_stat(base_pkmn bpkmn, int stat_index, int level)
{
    //IV = 0
    //EV = 0

    return ceil(((((2*(bpkmn.get_base_stats()[stat_index])) * level)/100) + 5) * 0.9);
}

int return_max_possible_stat(base_pkmn bpkmn, int stat_index, int level)
{
    //IV = 31
    //EV = 255

    return ceil(((((31 + 2*(bpkmn.get_base_stats()[stat_index]) + 63.75) * level)/100) + 5) * 1.1);
}

bool is_stat_possible(base_pkmn bpkmn, int stat_value, int stat_index, int level)
{
    return (stat_value > return_min_possible_stat(bpkmn,stat_index,level) and stat_value < return_max_possible_stat(bpkmn,stat_index,level));
}
