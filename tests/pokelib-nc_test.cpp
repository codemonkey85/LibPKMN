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

#include "../lib/library_bridge.hpp"
#include "../lib/conversions/pokemon.hpp"

/*
 * Test Pokemon-sim <-> PokeLib-NC conversion.
 */

BOOST_AUTO_TEST_CASE(pkmnsim_to_pokelib)
{
    pkmnsim::team_pokemon::sptr pkmnsim_pkmn = pkmnsim::team_pokemon::make(pkmnsim::Species::PIKACHU,
                                                                           pkmnsim::Games::DIAMOND,
                                                                           50,
                                                                           pkmnsim::Moves::THUNDER,
                                                                           pkmnsim::Moves::AGILITY,
                                                                           pkmnsim::Moves::VOLT_TACKLE,
                                                                           pkmnsim::Moves::SHOCK_WAVE);

    PokeLib::Pokemon pokelib_pkmn = pkmnsim::conversions::team_pokemon_to_pokelib_pokemon(pkmnsim_pkmn);

    pkmnsim::dict<unsigned int, unsigned int> stats = pkmnsim_pkmn->get_stats();
    pkmnsim::dict<unsigned int, unsigned int> EVs = pkmnsim_pkmn->get_EVs();
    pkmnsim::dict<unsigned int, unsigned int> IVs = pkmnsim_pkmn->get_IVs();
    uint32_t* IVint = &(pokelib_pkmn.pkm->pkm.IVint);

    std::cout << std::endl << "LibPKMNsim -> PokeLib-NC Effort Values" << std::endl;
    std::cout << " * HP:              " << EVs[pkmnsim::Stats::HP] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_hp) << std::endl;
    std::cout << " * Attack:          " << EVs[pkmnsim::Stats::ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_atk) << std::endl;
    std::cout << " * Defense:         " << EVs[pkmnsim::Stats::DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_def) << std::endl;
    std::cout << " * Speed:           " << EVs[pkmnsim::Stats::SPEED] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_spd) << std::endl;
    std::cout << " * Special Attack:  " << EVs[pkmnsim::Stats::SPECIAL_ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_satk) << std::endl;
    std::cout << " * Special Defense: " << EVs[pkmnsim::Stats::SPECIAL_DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_sdef) << std::endl;

    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_hp, EVs[pkmnsim::Stats::HP]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_atk, EVs[pkmnsim::Stats::ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_def, EVs[pkmnsim::Stats::DEFENSE]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_spd, EVs[pkmnsim::Stats::SPEED]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_satk, EVs[pkmnsim::Stats::SPECIAL_ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_sdef, EVs[pkmnsim::Stats::SPECIAL_DEFENSE]);

    std::cout << std::endl << "LibPKMNsim -> PokeLib-NC Individual Values" << std::endl;
    std::cout << " * HP:              " << IVs[pkmnsim::Stats::HP] << " -> " << int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::HP)) << std::endl;
    std::cout << " * Attack:          " << IVs[pkmnsim::Stats::ATTACK] << " -> " << int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::ATTACK)) << std::endl;
    std::cout << " * Defense:         " << IVs[pkmnsim::Stats::DEFENSE] << " -> " << int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::DEFENSE)) << std::endl;
    std::cout << " * Speed:           " << IVs[pkmnsim::Stats::SPEED] << " -> " << int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::SPEED)) << std::endl;
    std::cout << " * Special Attack:  " << IVs[pkmnsim::Stats::SPECIAL_ATTACK] << " -> " << int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::SPECIAL_ATTACK)) << std::endl;
    std::cout << " * Special Defense: " << IVs[pkmnsim::Stats::SPECIAL_DEFENSE] << " -> " << int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::SPECIAL_DEFENSE)) << std::endl;

    BOOST_CHECK_EQUAL(int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::HP)), IVs[pkmnsim::Stats::HP]);
    BOOST_CHECK_EQUAL(int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::ATTACK)), IVs[pkmnsim::Stats::ATTACK]);
    BOOST_CHECK_EQUAL(int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::DEFENSE)), IVs[pkmnsim::Stats::DEFENSE]);
    BOOST_CHECK_EQUAL(int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::SPEED)), IVs[pkmnsim::Stats::SPEED]);
    BOOST_CHECK_EQUAL(int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::SPECIAL_ATTACK)), IVs[pkmnsim::Stats::SPECIAL_ATTACK]);
    BOOST_CHECK_EQUAL(int(pkmnsim::modern_get_IV(IVint, pkmnsim::Stats::SPECIAL_DEFENSE)), IVs[pkmnsim::Stats::SPECIAL_DEFENSE]);

    std::cout << std::endl << "LibPKMNsim -> PokeLib-NC Stats" << std::endl;
    std::cout << " * HP:              " << stats[pkmnsim::Stats::HP] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_max_hp) << std::endl;
    std::cout << " * Attack:          " << stats[pkmnsim::Stats::ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_atk) << std::endl;
    std::cout << " * Defense:         " << stats[pkmnsim::Stats::DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_def) << std::endl;
    std::cout << " * Speed:           " << stats[pkmnsim::Stats::SPEED] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_spd) << std::endl;
    std::cout << " * Special Attack:  " << stats[pkmnsim::Stats::SPECIAL_ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_satk) << std::endl;
    std::cout << " * Special Defense: " << stats[pkmnsim::Stats::SPECIAL_DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_sdef) << std::endl;

    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_max_hp, stats[pkmnsim::Stats::HP]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_atk, stats[pkmnsim::Stats::ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_def, stats[pkmnsim::Stats::DEFENSE]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_spd, stats[pkmnsim::Stats::SPEED]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_satk, stats[pkmnsim::Stats::SPECIAL_ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_sdef, stats[pkmnsim::Stats::SPECIAL_DEFENSE]);
}
