/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ANALYSIS_HPP
#define INCLUDED_ANALYSIS_HPP

#include <string>
#include <vector>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*
     * Returns the minimum possible stat of a Pokemon.
     * Uses 0 EV, 0 IV, and disadvantageous nature (Gen 3-5).
     *
     * Parameters:
     *  - b_pkmn: Base Pokemon whose stats to analyze
     *  - stat: HP, ATK, DEF, SPD, SPCL, SATK, or SDEF
     *  - level: Pokemon's level for analysis
     *  - gen: Generation 1-5
     */
    int PKMNSIM_API get_min_possible_stat(base_pkmn::sptr b_pkmn, std::string stat, int level, int gen);

    /*
     * Returns the maximum possible stat of a Pokemon.
     * Uses max EV, max IV, and advantageous nature (Gen 3-5).
     *
     * Parameters:
     *  - b_pkmn: Base Pokemon whose stats to analyze
     *  - stat: HP, ATK, DEF, SPD, SPCL, SATK, or SDEF
     *  - level: Pokemon's level for analysis
     *  - gen: Generation 1-5
     */
    int PKMNSIM_API get_max_possible_stat(base_pkmn::sptr b_pkmn, std::string stat, int level, int gen);

    /*
     * Checks validity of specified statistic by using above functions.
     *
     * Parameters:
     *  - b_pkmn: Base Pokemon whose stats to analyze
     *  - stat: HP, ATK, DEF, SPD, SPCL, SATK, or SDEF
     *  - level: Pokemon's level for analysis
     *  - gen: Generation 1-5
     */
    bool PKMNSIM_API is_stat_possible(base_pkmn::sptr b_pkmn, int, std::string stat, int level, int gen);

    /*
     * Gets damage of a move, before taking type advantages and STAB.
     *
     * Parameters:
     *  - level: Pokemon's level for analysis
     *  - attack: Attacker's ATK/SPCL/SATK stat, as appropriate
     *  - defense: Defender's DEF/SPCL/SDEF stat, as appropriate
     *  - base_power: Move's base power
     */
    int PKMNSIM_API get_base_damage(int level, int attack, int defense, int base_power);


    /*
     * Gets range of damage of a move, taking type advantages and STAB into account.
     *
     * Parameters:
     *  - attacker: Base Pokemon executing move
     *  - defender: Base Pokemon receiving move
     *  - attacker_level: Attacker's level, taken into account in equation
     *  - gen: Generation of analysis
     *  - damage_range_vec: Reference to vector in which to return values
     */
    void PKMNSIM_API get_damage_range(base_pkmn::sptr attacker, base_pkmn::sptr defender,
                                      int attacker_level, int defender_level, int gen,
                                      std::vector<int>& damage_range_vec);
}

#endif /* INCLUDED_ANALYSIS_HPP */
