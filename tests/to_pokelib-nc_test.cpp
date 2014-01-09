/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/test/unit_test.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/team_pokemon.hpp>

#include <pokelib/pokelib.h>

#include "../lib/library_bridge.hpp"
#include "../lib/conversions/pokemon.hpp"

/*
 * Test Pokemon-sim <-> PokeLib-NC conversion.
 */

BOOST_AUTO_TEST_CASE(pkmnsim_to_pokelib)
{
    pkmn::team_pokemon::sptr pkmnsim_pkmn = pkmn::team_pokemon::make(pkmn::Species::ODDISH,
                                                                           pkmn::Games::DIAMOND,
                                                                           50,
                                                                           pkmn::Moves::VINE_WHIP,
                                                                           pkmn::Moves::POISONPOWDER,
                                                                           pkmn::Moves::RAZOR_LEAF,
                                                                           pkmn::Moves::SOLARBEAM);
    pkmnsim_pkmn->set_EV(pkmn::Stats::HP, 100);
    pkmnsim_pkmn->set_EV(pkmn::Stats::ATTACK, 100);
    pkmnsim_pkmn->set_EV(pkmn::Stats::DEFENSE, 100);
    pkmnsim_pkmn->set_EV(pkmn::Stats::SPEED, 100);
    pkmnsim_pkmn->set_EV(pkmn::Stats::SPECIAL_ATTACK, 100);
    pkmnsim_pkmn->set_EV(pkmn::Stats::SPECIAL_DEFENSE, 100);
    pkmnsim_pkmn->set_IV(pkmn::Stats::HP, 20);
    pkmnsim_pkmn->set_IV(pkmn::Stats::ATTACK, 20);
    pkmnsim_pkmn->set_IV(pkmn::Stats::DEFENSE, 20);
    pkmnsim_pkmn->set_IV(pkmn::Stats::SPEED, 20);
    pkmnsim_pkmn->set_IV(pkmn::Stats::SPECIAL_ATTACK, 20);
    pkmnsim_pkmn->set_IV(pkmn::Stats::SPECIAL_DEFENSE, 20);

    PokeLib::Pokemon pokelib_pkmn = pkmn::conversions::team_pokemon_to_pokelib_pokemon(pkmnsim_pkmn);

    pkmn::dict<unsigned int, unsigned int> stats = pkmnsim_pkmn->get_stats();
    pkmn::dict<unsigned int, unsigned int> EVs = pkmnsim_pkmn->get_EVs();
    pkmn::dict<unsigned int, unsigned int> IVs = pkmnsim_pkmn->get_IVs();
    uint32_t* IVint = &(pokelib_pkmn.pkm->pkm.IVint);

    std::cout << std::endl << "LibPKMN -> PokeLib-NC Effort Values" << std::endl;
    std::cout << " * HP:              " << EVs[pkmn::Stats::HP] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_hp) << std::endl;
    std::cout << " * Attack:          " << EVs[pkmn::Stats::ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_atk) << std::endl;
    std::cout << " * Defense:         " << EVs[pkmn::Stats::DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_def) << std::endl;
    std::cout << " * Speed:           " << EVs[pkmn::Stats::SPEED] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_spd) << std::endl;
    std::cout << " * Special Attack:  " << EVs[pkmn::Stats::SPECIAL_ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_satk) << std::endl;
    std::cout << " * Special Defense: " << EVs[pkmn::Stats::SPECIAL_DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.ev_sdef) << std::endl;

    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_hp, EVs[pkmn::Stats::HP]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_atk, EVs[pkmn::Stats::ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_def, EVs[pkmn::Stats::DEFENSE]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_spd, EVs[pkmn::Stats::SPEED]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_satk, EVs[pkmn::Stats::SPECIAL_ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.ev_sdef, EVs[pkmn::Stats::SPECIAL_DEFENSE]);

    std::cout << std::endl << "LibPKMN -> PokeLib-NC Individual Values" << std::endl;
    std::cout << " * HP:              " << IVs[pkmn::Stats::HP] << " -> " << int(pkmn::modern_get_IV(IVint, pkmn::Stats::HP)) << std::endl;
    std::cout << " * Attack:          " << IVs[pkmn::Stats::ATTACK] << " -> " << int(pkmn::modern_get_IV(IVint, pkmn::Stats::ATTACK)) << std::endl;
    std::cout << " * Defense:         " << IVs[pkmn::Stats::DEFENSE] << " -> " << int(pkmn::modern_get_IV(IVint, pkmn::Stats::DEFENSE)) << std::endl;
    std::cout << " * Speed:           " << IVs[pkmn::Stats::SPEED] << " -> " << int(pkmn::modern_get_IV(IVint, pkmn::Stats::SPEED)) << std::endl;
    std::cout << " * Special Attack:  " << IVs[pkmn::Stats::SPECIAL_ATTACK] << " -> " << int(pkmn::modern_get_IV(IVint, pkmn::Stats::SPECIAL_ATTACK)) << std::endl;
    std::cout << " * Special Defense: " << IVs[pkmn::Stats::SPECIAL_DEFENSE] << " -> " << int(pkmn::modern_get_IV(IVint, pkmn::Stats::SPECIAL_DEFENSE)) << std::endl;

    BOOST_CHECK_EQUAL(int(pkmn::modern_get_IV(IVint, pkmn::Stats::HP)), IVs[pkmn::Stats::HP]);
    BOOST_CHECK_EQUAL(int(pkmn::modern_get_IV(IVint, pkmn::Stats::ATTACK)), IVs[pkmn::Stats::ATTACK]);
    BOOST_CHECK_EQUAL(int(pkmn::modern_get_IV(IVint, pkmn::Stats::DEFENSE)), IVs[pkmn::Stats::DEFENSE]);
    BOOST_CHECK_EQUAL(int(pkmn::modern_get_IV(IVint, pkmn::Stats::SPEED)), IVs[pkmn::Stats::SPEED]);
    BOOST_CHECK_EQUAL(int(pkmn::modern_get_IV(IVint, pkmn::Stats::SPECIAL_ATTACK)), IVs[pkmn::Stats::SPECIAL_ATTACK]);
    BOOST_CHECK_EQUAL(int(pkmn::modern_get_IV(IVint, pkmn::Stats::SPECIAL_DEFENSE)), IVs[pkmn::Stats::SPECIAL_DEFENSE]);

    std::cout << std::endl << "LibPKMN -> PokeLib-NC Stats" << std::endl;
    std::cout << " * HP:              " << stats[pkmn::Stats::HP] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_max_hp) << std::endl;
    std::cout << " * Attack:          " << stats[pkmn::Stats::ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_atk) << std::endl;
    std::cout << " * Defense:         " << stats[pkmn::Stats::DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_def) << std::endl;
    std::cout << " * Speed:           " << stats[pkmn::Stats::SPEED] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_spd) << std::endl;
    std::cout << " * Special Attack:  " << stats[pkmn::Stats::SPECIAL_ATTACK] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_satk) << std::endl;
    std::cout << " * Special Defense: " << stats[pkmn::Stats::SPECIAL_DEFENSE] << " -> " << int(pokelib_pkmn.pkm->pkm.battle_sdef) << std::endl;

    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_max_hp, stats[pkmn::Stats::HP]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_atk, stats[pkmn::Stats::ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_def, stats[pkmn::Stats::DEFENSE]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_spd, stats[pkmn::Stats::SPEED]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_satk, stats[pkmn::Stats::SPECIAL_ATTACK]);
    BOOST_CHECK_EQUAL(pokelib_pkmn.pkm->pkm.battle_sdef, stats[pkmn::Stats::SPECIAL_DEFENSE]);
}
