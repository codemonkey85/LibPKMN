/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/test/unit_test.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/team_pokemon.hpp>

#include <pokelib/pokelib.h>

#include "../lib/conversions/pokemon.hpp"

/*
 * Test Pokemon-sim <-> PokeLib-NC conversion.
 */

BOOST_AUTO_TEST_CASE(pkmnsim_to_pokelib_DPPt)
{
    pkmnsim::team_pokemon::sptr pkmnsim_pkmn = pkmnsim::team_pokemon::make(pkmnsim::Species::PIKACHU,
                                                                           pkmnsim::Games::DIAMOND,
                                                                           50,
                                                                           pkmnsim::Moves::THUNDER,
                                                                           pkmnsim::Moves::AGILITY,
                                                                           pkmnsim::Moves::VOLT_TACKLE,
                                                                           pkmnsim::Moves::SHOCK_WAVE);

    PokeLib::Pokemon pokelib_pkmn = pkmnsim::conversions::team_pokemon_to_pokelib_pokemon(pkmnsim_pkmn);

    pokelib_pkmn.updateBattleStats();
    pkmnsim::dict<unsigned int, unsigned int> stats = pkmnsim_pkmn->get_stats();

    std::cout << std::endl << "PokeLib vs. LibPKMNsim stats" << std::endl;
    std::cout << " * " << int(pokelib_pkmn.pkm->pkm.battle_max_hp) << " - " << stats[pkmnsim::Stats::HP] << std::endl;
    std::cout << " * " << int(pokelib_pkmn.pkm->pkm.battle_atk) << " - " << stats[pkmnsim::Stats::ATTACK] << std::endl;
    std::cout << " * " << int(pokelib_pkmn.pkm->pkm.battle_def) << " - " << stats[pkmnsim::Stats::DEFENSE] << std::endl;
    std::cout << " * " << int(pokelib_pkmn.pkm->pkm.battle_satk) << " - " << stats[pkmnsim::Stats::SPECIAL_ATTACK] << std::endl;
    std::cout << " * " << int(pokelib_pkmn.pkm->pkm.battle_sdef) << " - " << stats[pkmnsim::Stats::SPECIAL_DEFENSE] << std::endl;
    std::cout << " * " << int(pokelib_pkmn.pkm->pkm.battle_spd) << " - " << stats[pkmnsim::Stats::SPEED] << std::endl;

    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_max_hp, stats[pkmnsim::Stats::HP]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_atk, stats[pkmnsim::Stats::ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_def, stats[pkmnsim::Stats::DEFENSE]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_satk, stats[pkmnsim::Stats::SPECIAL_ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_sdef, stats[pkmnsim::Stats::SPECIAL_DEFENSE]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_spd, stats[pkmnsim::Stats::SPEED]);
}
