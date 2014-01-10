/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/database/queries.hpp>

#include <pokelib/pokelib.h>

#include "../lib/library_bridge.hpp"
#include "../lib/conversions/pokemon.hpp"

/*
 * Test Pokemon-sim <-> PokeLib-NC conversion.
 */

BOOST_AUTO_TEST_CASE(libpkmn_to_pokelib)
{
    pkmn::team_pokemon::sptr libpkmn_pkmn = pkmn::team_pokemon::make(pkmn::Species::ODDISH,
                                                                     pkmn::Games::DIAMOND,
                                                                     50,
                                                                     pkmn::Moves::VINE_WHIP,
                                                                     pkmn::Moves::POISONPOWDER,
                                                                     pkmn::Moves::RAZOR_LEAF,
                                                                     pkmn::Moves::SOLARBEAM);
    libpkmn_pkmn->set_EV(pkmn::Stats::HP, 100);
    libpkmn_pkmn->set_EV(pkmn::Stats::ATTACK, 100);
    libpkmn_pkmn->set_EV(pkmn::Stats::DEFENSE, 100);
    libpkmn_pkmn->set_EV(pkmn::Stats::SPEED, 100);
    libpkmn_pkmn->set_EV(pkmn::Stats::SPECIAL_ATTACK, 100);
    libpkmn_pkmn->set_EV(pkmn::Stats::SPECIAL_DEFENSE, 100);
    libpkmn_pkmn->set_IV(pkmn::Stats::HP, 20);
    libpkmn_pkmn->set_IV(pkmn::Stats::ATTACK, 20);
    libpkmn_pkmn->set_IV(pkmn::Stats::DEFENSE, 20);
    libpkmn_pkmn->set_IV(pkmn::Stats::SPEED, 20);
    libpkmn_pkmn->set_IV(pkmn::Stats::SPECIAL_ATTACK, 20);
    libpkmn_pkmn->set_IV(pkmn::Stats::SPECIAL_DEFENSE, 20);
    libpkmn_pkmn->set_held_item(pkmn::Items::PECHA_BERRY);

    PokeLib::Pokemon pokelib_pkmn = pkmn::conversions::team_pokemon_to_pokelib_pokemon(libpkmn_pkmn);

    pkmn::dict<unsigned int, unsigned int> stats = libpkmn_pkmn->get_stats();
    pkmn::dict<unsigned int, unsigned int> EVs = libpkmn_pkmn->get_EVs();
    pkmn::dict<unsigned int, unsigned int> IVs = libpkmn_pkmn->get_IVs();
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

    std::string libpkmn_nickname = libpkmn_pkmn->get_nickname();
    std::string pokelib_nickname = pkmn::pokemon_text(pokelib_pkmn.getNickname()).std_string();
    std::string libpkmn_trainer_name = libpkmn_pkmn->get_trainer_name();
    std::string pokelib_trainer_name = pkmn::pokemon_text(pokelib_pkmn.getTrainerName()).std_string();
    unsigned int pokelib_pkmn_item_id = pkmn::database::get_item_index(pokelib_pkmn.pkm->pkm.held_item, pkmn::Games::DIAMOND);
    std::string pokelib_pkmn_item_name = pkmn::database::get_item_name(pokelib_pkmn_item_id);

    std::cout << std::endl << "LibPKMN -> PokeLib-NC - Other" << std::endl;
    std::cout << (boost::format(" * Nickname: \"%s\" -> \"%s\"\n") % libpkmn_nickname % pokelib_nickname);
    BOOST_CHECK_EQUAL(libpkmn_nickname.size(), pokelib_nickname.size());

    std::cout << (boost::format(" * Nickname: \"%s\" -> \"%s\"\n") % libpkmn_trainer_name % pokelib_trainer_name);
    BOOST_CHECK_EQUAL(libpkmn_trainer_name.size(), pokelib_trainer_name.size());

    std::cout << " * Held Item: " << pkmn::database::get_item_name(libpkmn_pkmn->get_held_item()->get_item_id()) << " -> " << pokelib_pkmn_item_name << std::endl;
    BOOST_CHECK_EQUAL(libpkmn_pkmn->get_held_item()->get_item_id(), pokelib_pkmn_item_id);
    std::cout << " * Personality: " << libpkmn_pkmn->get_personality() << " -> " << int(pokelib_pkmn.pkm->pkm.pid) << std::endl;
    BOOST_CHECK_EQUAL(libpkmn_pkmn->get_personality(), pokelib_pkmn.pkm->pkm.pid);
}
