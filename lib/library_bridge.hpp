/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LIBRARY_BRIDGE_HPP
#define INCLUDED_LIBRARY_BRIDGE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <cstdint>

#include <boost/assign.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/types/dict.hpp>

#include <pokehack/SaveParser.h>

#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g6.h>

namespace pkmnsim
{
    static const dict<unsigned int, unsigned int> rpokesav_gen1_index_map = boost::assign::map_list_of
        (1, Species::RHYDON)
        (2, Species::KANGASKHAN)
        (3, Species::NIDORAN_M)
        (4, Species::CLEFAIRY)
        (5, Species::SPEAROW)
        (6, Species::VOLTORB)
        (7, Species::NIDOKING)
        (8, Species::SLOWBRO)
        (9, Species::IVYSAUR)
        (10, Species::EXEGGUTOR)
        (11, Species::LICKITUNG)
        (12, Species::EXEGGCUTE)
        (13, Species::GRIMER)
        (14, Species::GENGAR)
        (15, Species::NIDORAN_F)
        (16, Species::NIDOQUEEN)
        (17, Species::CUBONE)
        (18, Species::RHYHORN)
        (19, Species::LAPRAS)
        (20, Species::ARCANINE)
        (21, Species::MEW)
        (22, Species::GYARADOS)
        (23, Species::SHELLDER)
        (24, Species::TENTACOOL)
        (25, Species::GASTLY)
        (26, Species::SCYTHER)
        (27, Species::STARYU)
        (28, Species::BLASTOISE)
        (29, Species::PINSIR)
        (30, Species::TANGELA)
        (31, Species::INVALID)
        (32, Species::INVALID)
        (33, Species::GROWLITHE)
        (34, Species::ONIX)
        (35, Species::FEAROW)
        (36, Species::PIDGEY)
        (37, Species::SLOWPOKE)
        (38, Species::KADABRA)
        (39, Species::GRAVELER)
        (40, Species::CHANSEY)
        (41, Species::MACHOKE)
        (42, Species::MR_MIME)
        (43, Species::HITMONLEE)
        (44, Species::HITMONCHAN)
        (45, Species::ARBOK)
        (46, Species::PARASECT)
        (47, Species::PSYDUCK)
        (48, Species::DROWZEE)
        (49, Species::GOLEM)
        (50, Species::INVALID)
        (51, Species::MAGMAR)
        (52, Species::INVALID)
        (53, Species::ELECTABUZZ)
        (54, Species::MAGNETON)
        (55, Species::KOFFING)
        (56, Species::INVALID)
        (57, Species::MANKEY)
        (58, Species::SEEL)
        (59, Species::DIGLETT)
        (60, Species::TAUROS)
        (61, Species::INVALID)
        (62, Species::INVALID)
        (63, Species::INVALID)
        (64, Species::FARFETCHD)
        (65, Species::VENONAT)
        (66, Species::DRAGONITE)
        (67, Species::INVALID)
        (68, Species::INVALID)
        (69, Species::INVALID)
        (70, Species::DODUO)
        (71, Species::POLIWAG)
        (72, Species::JYNX)
        (73, Species::MOLTRES)
        (74, Species::ARTICUNO)
        (75, Species::ZAPDOS)
        (76, Species::DITTO)
        (77, Species::MEOWTH)
        (78, Species::KRABBY)
        (79, Species::INVALID)
        (80, Species::INVALID)
        (81, Species::INVALID)
        (82, Species::VULPIX)
        (83, Species::NINETALES)
        (84, Species::PIKACHU)
        (85, Species::RAICHU)
        (86, Species::INVALID)
        (87, Species::INVALID)
        (88, Species::DRATINI)
        (89, Species::DRAGONAIR)
        (90, Species::KABUTO)
        (91, Species::KABUTOPS)
        (92, Species::HORSEA)
        (93, Species::SEADRA)
        (94, Species::INVALID)
        (95, Species::INVALID)
        (96, Species::SANDSHREW)
        (97, Species::SANDSLASH)
        (98, Species::OMANYTE)
        (99, Species::OMASTAR)
        (100, Species::JIGGLYPUFF)
        (101, Species::WIGGLYTUFF)
        (102, Species::EEVEE)
        (103, Species::FLAREON)
        (104, Species::JOLTEON)
        (105, Species::VAPOREON)
        (106, Species::MACHOP)
        (107, Species::ZUBAT)
        (108, Species::EKANS)
        (109, Species::PARAS)
        (110, Species::POLIWHIRL)
        (111, Species::POLIWRATH)
        (112, Species::WEEDLE)
        (113, Species::KAKUNA)
        (114, Species::BEEDRILL)
        (115, Species::INVALID)
        (116, Species::DODRIO)
        (117, Species::PRIMEAPE)
        (118, Species::DUGTRIO)
        (119, Species::VENOMOTH)
        (120, Species::DEWGONG)
        (121, Species::INVALID)
        (122, Species::INVALID)
        (123, Species::CATERPIE)
        (124, Species::METAPOD)
        (125, Species::BUTTERFREE)
        (126, Species::MACHAMP)
        (127, Species::INVALID)
        (128, Species::GOLDUCK)
        (129, Species::HYPNO)
        (130, Species::GOLBAT)
        (131, Species::MEWTWO)
        (132, Species::SNORLAX)
        (133, Species::MAGIKARP)
        (134, Species::INVALID)
        (135, Species::INVALID)
        (136, Species::MUK)
        (137, Species::INVALID)
        (138, Species::KINGLER)
        (139, Species::CLOYSTER)
        (140, Species::INVALID)
        (141, Species::ELECTRODE)
        (142, Species::CLEFABLE)
        (143, Species::WEEZING)
        (144, Species::PERSIAN)
        (145, Species::MAROWAK)
        (146, Species::INVALID)
        (147, Species::HAUNTER)
        (148, Species::ABRA)
        (149, Species::ALAKAZAM)
        (150, Species::PIDGEOTTO)
        (151, Species::PIDGEOT)
        (152, Species::STARMIE)
        (153, Species::BULBASAUR)
        (154, Species::VENUSAUR)
        (155, Species::TENTACRUEL)
        (156, Species::INVALID)
        (157, Species::GOLDEEN)
        (158, Species::SEAKING)
        (159, Species::INVALID)
        (160, Species::INVALID)
        (161, Species::INVALID)
        (162, Species::INVALID)
        (163, Species::PONYTA)
        (164, Species::RAPIDASH)
        (165, Species::RATTATA)
        (166, Species::RATICATE)
        (167, Species::NIDORINO)
        (168, Species::NIDORINA)
        (169, Species::GEODUDE)
        (170, Species::PORYGON)
        (171, Species::AERODACTYL)
        (172, Species::INVALID)
        (173, Species::MAGNEMITE)
        (174, Species::INVALID)
        (175, Species::INVALID)
        (176, Species::CHARMANDER)
        (177, Species::SQUIRTLE)
        (178, Species::CHARMELEON)
        (179, Species::WARTORTLE)
        (180, Species::CHARIZARD)
        (181, Species::INVALID)
        (182, Species::INVALID)
        (183, Species::INVALID)
        (184, Species::INVALID)
        (185, Species::ODDISH)
        (186, Species::GLOOM)
        (187, Species::VILEPLUME)
        (188, Species::BELLSPROUT)
        (189, Species::WEEPINBELL)
        (190, Species::VICTREEBEL);

    static const dict<char, int> pokehack_reverse_char_map = boost::assign::map_list_of
        ('0', int(0xA1))
        ('1', int(0xA2))
        ('2', int(0xA3))
        ('3', int(0xA4))
        ('4', int(0xA5))
        ('5', int(0xA6))
        ('6', int(0xA7))
        ('7', int(0xA8))
        ('8', int(0xA9))
        ('9', int(0xAA))
        ('!', int(0xAB))
        ('?', int(0xAC))
        ('.', int(0xAD))
        ('-', int(0xAE))
        ('"', int(0xB1))
        ('\'', int(0xB2))
        ('/', int(0xBA))
        ('A', int(0xBB))
        ('B', int(0xBC))
        ('C', int(0xBD))
        ('D', int(0xBE))
        ('E', int(0xBF))
        ('F', int(0xC0))
        ('G', int(0xC1))
        ('H', int(0xC2))
        ('I', int(0xC3))
        ('J', int(0xC4))
        ('K', int(0xC5))
        ('L', int(0xC6))
        ('M', int(0xC7))
        ('N', int(0xC8))
        ('O', int(0xC9))
        ('P', int(0xCA))
        ('Q', int(0xCB))
        ('R', int(0xCC))
        ('S', int(0xCD))
        ('T', int(0xCE))
        ('U', int(0xCF))
        ('V', int(0xD0))
        ('W', int(0xD1))
        ('X', int(0xD2))
        ('Y', int(0xD3))
        ('Z', int(0xD4))
        ('a', int(0xD5))
        ('b', int(0xD6))
        ('c', int(0xD7))
        ('d', int(0xD8))
        ('e', int(0xD9))
        ('f', int(0xDA))
        ('g', int(0xDB))
        ('h', int(0xDC))
        ('i', int(0xDD))
        ('j', int(0xDE))
        ('k', int(0xDF))
        ('l', int(0xE0))
        ('m', int(0xE1))
        ('n', int(0xE2))
        ('o', int(0xE3))
        ('p', int(0xE4))
        ('q', int(0xE5))
        ('r', int(0xE6))
        ('s', int(0xE7))
        ('t', int(0xE8))
        ('u', int(0xE9))
        ('v', int(0xEA))
        ('w', int(0xEB))
        ('x', int(0xEC))
        ('y', int(0xED))
        ('z', int(0xEE))
        (' ', int(0xEF))
        ('\0', int(0xFF));

    char* pokehack_get_text(unsigned char* raw, bool is_nickname);

    uint8_t modern_get_IV(uint32_t* IVint, uint8_t IV);

    void modern_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val);
    
    bool get_marking(uint8_t* markint, uint8_t mark);
    
    void set_marking(uint8_t* markint, uint8_t mark, bool val);
    
    bool get_ribbon(uint32_t* ribbonint, uint8_t ribbon);
    
    void set_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val);

    uint8_t get_gen3_ball(uint16_t* metlevelint);

    void set_gen3_ball(uint16_t* metlevelint, uint8_t ball);

    uint8_t get_gen3_met_level(uint16_t* metlevelint);

    void set_gen3_met_level(uint16_t* metlevelint, uint8_t level);

    uint8_t get_gen_456_met_level(uint8_t* metlevelint);

    void set_gen_456_met_level(uint8_t* metlevelint, uint8_t level);

    bool get_gen3_otgender(uint16_t* metlevelint);

    void set_gen3_otgender(uint16_t* metlevelint, bool is_female);

    bool get_gen_456_otgender(uint8_t* metlevelint);

    void set_gen_456_otgender(uint8_t* metlevelint, bool is_female);
    
    uint8_t pkmnsim_getpkmstat(pokemon_obj* pkm);
    
    void pkmnsim_pctoparty(party_pkm* p_pkm, pokemon_obj* pkm);
    
    std::string pkmnsim_getpkxformnamesql(pokemonx_obj *pkx);
    
    uint16_t getpkxstat(pokemonx_obj *pkx, unsigned int stat_id);
    
    uint8_t pkmnsim_pkxstat(pokemonx_obj* pkx);
    
    void pkmnsim_pctopartyx(party_pkx* p_pkx, pokemonx_obj* pkx);

    uint8_t pkmnsim_game_to_hometown(uint8_t game);

    uint8_t hometown_to_pkmnsim_game(uint8_t hometown);

    uint8_t pkmnsim_ball_to_game_ball(uint8_t ball);
    
    uint8_t game_ball_to_pkmnsim_ball(uint8_t game_ball);

    namespace Hometowns
    {
        enum hometowns
        {
            NONE,
            SAPPHIRE,
            RUBY,
            EMERALD,
            FIRE_RED,
            LEAF_GREEN,
            HEART_GOLD = 7,
            SOUL_SILVER,
            DIAMOND = 10,
            PEARL,
            PLATINUM,
            COLOSSEUM_XD = 15,
            WHITE = 20,
            BLACK,
            WHITE2,
            BLACK2,
            X,
            Y
        };
    }
}

#endif /* INCLUDED_LIBRARY_BRIDGE_HPP */
