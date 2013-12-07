/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif
 
#include <bitset> 

#include <boost/assign.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include <pokehack/pokestructs.h>
#include <pkmds/pkmds_sql.h>

#include "library_bridge.hpp"
#include "SQLiteCpp/src/SQLiteC++.h"

#define MAX_NICKNAME_LEN 10
#define MAX_TRAINER_NAME_LEN 7

using namespace std;

namespace pkmnsim
{
    dict<char, int> get_pokehack_reverse_char_map()
    {
        dict<char, int> pokehack_reverse_char_map = boost::assign::map_list_of
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
            
        return pokehack_reverse_char_map;
    }

    //To avoid C/C++ include messiness while avoiding bringing in pokehack/SaveParser.h
    char* pokehack_get_text(unsigned char* raw, bool is_nickname)
    {
        char* actual_text;
        int len;

        if(is_nickname)
        {
            actual_text = new char[MAX_NICKNAME_LEN];
            len = MAX_NICKNAME_LEN;
        }
        else
        {
            actual_text = new char[MAX_TRAINER_NAME_LEN];
            len = MAX_TRAINER_NAME_LEN;
        }

        for(int i = 0; i < len; i++)
        {
            if(int(raw[i]) != 255) actual_text[i] = text[int(raw[i])];
            else actual_text[i] = '\0';
        }

        return actual_text;
    }

    uint8_t modern_get_IV(uint32_t* IVint, uint8_t IV)
    {
        bitset<32> IVs_bitset = int(*IVint);
        bitset<5> end_bitset = 0;
    
        switch(IV)
        {
            case Stats::HP:
                end_bitset[0] = IVs_bitset[27];
                end_bitset[1] = IVs_bitset[28];
                end_bitset[2] = IVs_bitset[29];
                end_bitset[3] = IVs_bitset[30];
                end_bitset[4] = IVs_bitset[31];
                break;
                
            case Stats::ATTACK:
                end_bitset[0] = IVs_bitset[22];
                end_bitset[1] = IVs_bitset[23];
                end_bitset[2] = IVs_bitset[24];
                end_bitset[3] = IVs_bitset[25];
                end_bitset[4] = IVs_bitset[26];
                break;
            
            case Stats::DEFENSE:
                end_bitset[0] = IVs_bitset[17];
                end_bitset[1] = IVs_bitset[18];
                end_bitset[2] = IVs_bitset[19];
                end_bitset[3] = IVs_bitset[20];
                end_bitset[4] = IVs_bitset[21];
                break;
                
            case Stats::SPEED:
                end_bitset[0] = IVs_bitset[12];
                end_bitset[1] = IVs_bitset[13];
                end_bitset[2] = IVs_bitset[14];
                end_bitset[3] = IVs_bitset[15];
                end_bitset[4] = IVs_bitset[16];
                break;
                
            case Stats::SPECIAL_ATTACK:
                end_bitset[0] = IVs_bitset[7];
                end_bitset[1] = IVs_bitset[8];
                end_bitset[2] = IVs_bitset[9];
                end_bitset[3] = IVs_bitset[10];
                end_bitset[4] = IVs_bitset[11];
                break;
                
            case Stats::SPECIAL_DEFENSE:
                end_bitset[0] = IVs_bitset[2];
                end_bitset[1] = IVs_bitset[3];
                end_bitset[2] = IVs_bitset[4];
                end_bitset[3] = IVs_bitset[5];
                end_bitset[4] = IVs_bitset[6];
                break;
                
            default:
                return 0;
        }
        return end_bitset.to_ulong();
    }
    
    void modern_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val)
    {
        bitset<32> IVs_bitset = int(*IVint);
        bitset<5> val_bitset = int(val);
    
        switch(IV)
        {
            case Stats::HP:
                IVs_bitset[27] = val_bitset[0];
                IVs_bitset[28] = val_bitset[1];
                IVs_bitset[29] = val_bitset[2];
                IVs_bitset[30] = val_bitset[3];
                IVs_bitset[31] = val_bitset[4];
                break;
                
            case Stats::ATTACK:
                IVs_bitset[22] = val_bitset[0];
                IVs_bitset[23] = val_bitset[1];
                IVs_bitset[24] = val_bitset[2];
                IVs_bitset[25] = val_bitset[3];
                IVs_bitset[26] = val_bitset[4];
                break;
            
            case Stats::DEFENSE:
                IVs_bitset[17] = val_bitset[0];
                IVs_bitset[18] = val_bitset[1];
                IVs_bitset[19] = val_bitset[2];
                IVs_bitset[20] = val_bitset[3];
                IVs_bitset[21] = val_bitset[4];
                break;
                
            case Stats::SPEED:
                IVs_bitset[12] = val_bitset[0];
                IVs_bitset[13] = val_bitset[1];
                IVs_bitset[14] = val_bitset[2];
                IVs_bitset[15] = val_bitset[3];
                IVs_bitset[16] = val_bitset[4];
                break;
                
            case Stats::SPECIAL_ATTACK:
                IVs_bitset[7] = val_bitset[0];
                IVs_bitset[8] = val_bitset[1];
                IVs_bitset[9] = val_bitset[2];
                IVs_bitset[10] = val_bitset[3];
                IVs_bitset[11] = val_bitset[4];
                break;
                
            case Stats::SPECIAL_DEFENSE:
                IVs_bitset[2] = val_bitset[0];
                IVs_bitset[3] = val_bitset[1];
                IVs_bitset[4] = val_bitset[2];
                IVs_bitset[5] = val_bitset[3];
                IVs_bitset[6] = val_bitset[4];
                break;
        }
        *IVint = IVs_bitset.to_ulong();
    }
    
    bool get_marking(uint8_t* markint, uint8_t mark)
    {
        bitset<8> marking_bitset = int(*markint);
        return marking_bitset[mark];
    }
    
    void set_marking(uint8_t* markint, uint8_t mark, bool val)
    {
        bitset<8> marking_bitset = int(*markint);
        marking_bitset[mark] = val;
        *markint = marking_bitset.to_ulong();
    }
    
    bool get_ribbon(uint32_t* ribbonint, uint8_t ribbon)
    {
        bitset<32> ribbon_bitset = int(*ribbonint);
        return ribbon_bitset[ribbon];
    }
    
    void set_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val)
    {
        if(val) *ribbonint |= (1 << ribbon);
        else *ribbonint &= ~(1 << ribbon);
    }

    uint8_t get_gen3_ball(uint16_t* metlevelint)
    {
        bitset<16> metlevel_bitset = int(*metlevelint);
        bitset<8> ball_bitset = 0;
        for(int i = 0; i < 4; i++) ball_bitset[i] = metlevel_bitset[i+1];

        return ball_bitset.to_ulong();
    }

    void set_gen3_ball(uint16_t* metlevelint, uint8_t ball)
    {
        bitset<16> metlevel_bitset = int(*metlevelint);
        bitset<8> ball_bitset = ball;
        for(int i = 0; i < 4; i++) metlevel_bitset[i+1] = ball_bitset[i];

        *metlevelint = metlevel_bitset.to_ulong();
    }

    uint8_t get_gen3_met_level(uint16_t* metlevelint)
    {
        bitset<16> metlevel_bitset = int(*metlevelint);
        bitset<8> metlevel_int = 0;
        for(int i = 15; i > 8; i--) metlevel_int[i-8] = metlevel_bitset[i];

        return metlevel_int.to_ulong();
    }

    void set_gen3_met_level(uint16_t* metlevelint, uint8_t level)
    {
        bitset<16> metlevel_bitset = int(*metlevelint);
        bitset<8> metlevel_int = level;
        for(int i = 0; i < 7; i++) metlevel_bitset[i+8] = metlevel_int[i];

        *metlevelint = metlevel_bitset.to_ulong();
    }

    uint8_t get_gen_456_met_level(uint8_t* metlevelint)
    {
        return (*metlevelint >> 1);
    }

    void set_gen_456_met_level(uint8_t* metlevelint, uint8_t level)
    {
        bitset<8> metlevel_bitset = int(*metlevelint);
        bitset<8> level_bitset = level;
        for(int i = 0; i < 7; i++) metlevel_bitset[i+1] = level_bitset[i];

        *metlevelint = metlevel_bitset.to_ulong();
    }

    bool get_gen3_otgender(uint16_t* metlevelint)
    {
        bitset<16> metlevel_bitset = int(*metlevelint);
        return metlevel_bitset[0];
    }

    void set_gen3_otgender(uint16_t* metlevelint, bool is_female)
    {
        bitset<16> metlevel_bitset = int(*metlevelint);
        metlevel_bitset[0] = is_female;
        
        *metlevelint = metlevel_bitset.to_ulong();
    }

    bool get_gen_456_otgender(uint8_t* metlevelint)
    {
        bitset<8> metlevel_bitset = int(*metlevelint);
        return metlevel_bitset[0];
    }

    void set_gen_456_otgender(uint8_t* metlevelint, bool is_female)
    {
        bitset<8> metlevel_bitset = int(*metlevelint);
        metlevel_bitset[0] = is_female;
        *metlevelint = metlevel_bitset.to_ulong();
    }

    uint8_t pkmnsim_getpkmstat(pokemon_obj* pkm, unsigned int stat_id)
    {
        SQLite::Database db(get_database_path().c_str());
        std::string pkmstatsql = getpkmstatsql(pkm, ::Stat_IDs::stat_ids(stat_id));
        unsigned int basestat = int(db.execAndGet(pkmstatsql.c_str()));
                                     
        std::string pkmlevelsql = getpkmlevelsql(int(pkm->species),
                                                 int(pkm->exp));
        unsigned int level = int(db.execAndGet(pkmlevelsql.c_str()));
        unsigned int ev = 0;
        unsigned int iv = 0;
        
        uint32_t* IVint = reinterpret_cast<uint32_t*>(&(pkm->ppup[3])+1);
        switch(stat_id)
        {
            case Stats::HP:
                ev = pkm->evs.hp;
                iv = modern_get_IV(IVint, Stats::HP);
                return (int)((floor((double)(floor((double)(((iv +
                       (2 * basestat) + floor((double)(ev/4))+100)
                       * level) / 100)) + 10))));
                       
            case Stats::ATTACK:
                ev = pkm->evs.attack;
                break;
                
            case Stats::DEFENSE:
                ev = pkm->evs.defense;
                break;
                
            case Stats::SPECIAL_ATTACK:
                ev = pkm->evs.spatk;
                break;
                
            case Stats::SPECIAL_DEFENSE:
                ev = pkm->evs.spdef;
                break;
                
            default:
                ev = pkm->evs.speed;
        }
        iv = modern_get_IV(IVint, stat_id);
        double naturemod = database::get_nature_stat_effect(
                           (unsigned int)(pkm->nature), stat_id);
        return (int)((floor((double)(floor((double)(((iv + (2 * basestat)
               + floor((double)(ev/4))) * level) / 100)) + 5)) * naturemod));
    }
    
    void pkmnsim_pctoparty(party_pkm* p_pkm, pokemon_obj* pkm)
    {
        p_pkm->pkm_data = *pkm;
        p_pkm->party_data.maxhp = pkmnsim_getpkmstat(pkm, Stats::HP);
        p_pkm->party_data.hp = p_pkm->party_data.maxhp;
        p_pkm->party_data.attack = pkmnsim_getpkmstat(pkm, Stats::ATTACK);
        p_pkm->party_data.defense = pkmnsim_getpkmstat(pkm, Stats::DEFENSE);
        p_pkm->party_data.spatk = pkmnsim_getpkmstat(pkm, Stats::SPECIAL_ATTACK);
        p_pkm->party_data.spdef = pkmnsim_getpkmstat(pkm, Stats::SPECIAL_DEFENSE);
        p_pkm->party_data.speed = pkmnsim_getpkmstat(pkm, Stats::SPEED);
        
        SQLite::Database db(get_database_path().c_str());
        std::string pkxlevelsql = getpkmlevelsql(int(pkm->species),
                                                 int(pkm->exp));
        unsigned int level = int(db.execAndGet(pkxlevelsql.c_str()));
    }
    
    string pkmnsim_getpkxformnamesql(pokemonx_obj *pkx)
    {
        ostringstream o;
        o << ""
          << "SELECT pokemon_form_names.form_name "
          << "FROM   pokemon_forms "
          << "       INNER JOIN pokemon_form_names "
          << "               ON pokemon_forms.id = pokemon_form_names.pokemon_form_id "
          << "       INNER JOIN pokemon "
          << "               ON pokemon_forms.pokemon_id = pokemon.id "
          << "       INNER JOIN pokemon_species "
          << "               ON pokemon.species_id = pokemon_species.id "
          << "       INNER JOIN pokemon_species_names "
          << "               ON pokemon_species.id = pokemon_species_names.pokemon_species_id "
          << "WHERE  ( pokemon_form_names.local_language_id = 9 ) "
          << "       AND ( pokemon_species_names.local_language_id = 9 ) "
          << "       AND ( pokemon.species_id = " << (uint16)(pkx->species) << " ) "
          << "       AND ( pokemon_forms.form_order = " << (int)(pkx->forms.form) << " + 1 ) ";
        return o.str();
    }
    
    std::string pkmnsim_getpkxstatsql(pokemonx_obj *pkx, unsigned int stat_id)
    {
        ostringstream o;
        o << ""
          << "SELECT pokemon_stats.base_stat "
          << "FROM   pokemon_stats "
          << "       INNER JOIN pokemon_forms "
          << "               ON pokemon_stats.pokemon_id = pokemon_forms.pokemon_id "
          << "       INNER JOIN stats "
          << "               ON pokemon_stats.stat_id = stats.id "
          << "       INNER JOIN stat_names "
          << "               ON stats.id = stat_names.stat_id "
          << "       INNER JOIN pokemon_species_names "
          << "               ON stat_names.local_language_id = "
          << "                  pokemon_species_names.local_language_id "
          << "       INNER JOIN pokemon "
          << "               ON pokemon_stats.pokemon_id = pokemon.id "
          << "                  AND pokemon_forms.pokemon_id = pokemon.id "
          << "                  AND pokemon_species_names.pokemon_species_id = "
          << "                      pokemon.species_id "
          << "WHERE  ( pokemon_species_names.local_language_id = 9 ) "
          << "       AND ( stat_names.local_language_id = 9 ) "
          << "       AND ( pokemon_species_names.pokemon_species_id = " << (uint16)pkx->species << " ) ";
        if(pkmnsim_getpkxformnamesql(pkx) != "")
        {
            o << "       AND ( pokemon_forms.form_order = " << (int)(pkx->forms.form) << " + 1 ) ";
        }
        o << "       AND ( stat_names.stat_id = " << (int)stat_id << " ) ";
        return o.str();
    }

    uint8_t pkmnsim_getpkxstat(pokemonx_obj* pkx, unsigned int stat_id)
    {
        SQLite::Database db(get_database_path().c_str());
        std::string pkxstatsql = pkmnsim_getpkxstatsql(pkx, stat_id);
        unsigned int basestat = int(db.execAndGet(pkxstatsql.c_str()));
                                     
        std::string pkxlevelsql = getpkmlevelsql(int(pkx->species),
                                                 int(pkx->exp));
        unsigned int level = int(db.execAndGet(pkxlevelsql.c_str()));
        unsigned int ev = 0;
        unsigned int iv = 0;
        
        uint32_t* IVint = reinterpret_cast<uint32_t*>(&(pkx->ppups[3])+1);
        switch(stat_id)
        {
            case Stats::HP:
                ev = pkx->evs.hp;
                iv = modern_get_IV(IVint, Stats::HP);
                return (int)((floor((double)(floor((double)(((iv +
                       (2 * basestat) + floor((double)(ev/4))+100)
                       * level) / 100)) + 10))));
                       
            case Stats::ATTACK:
                ev = pkx->evs.attack;
                break;
                
            case Stats::DEFENSE:
                ev = pkx->evs.defense;
                break;
                
            case Stats::SPECIAL_ATTACK:
                ev = pkx->evs.spatk;
                break;
                
            case Stats::SPECIAL_DEFENSE:
                ev = pkx->evs.spdef;
                break;
                
            default:
                ev = pkx->evs.speed;
        }
        iv = modern_get_IV(IVint, stat_id);
        double naturemod = database::get_nature_stat_effect(
                           (unsigned int)(pkx->nature), stat_id);
        return (int)((floor((double)(floor((double)(((iv + (2 * basestat)
               + floor((double)(ev/4))) * level) / 100)) + 5)) * naturemod));
    }

void pkmnsim_pctopartyx(party_pkx* p_pkx, pokemonx_obj* pkx)
    {
    
        p_pkx->pkx_data = *pkx;
        p_pkx->partyx_data.maxhp = pkmnsim_getpkxstat(pkx, Stats::HP);
        p_pkx->partyx_data.hp = p_pkx->partyx_data.maxhp;
        p_pkx->partyx_data.attack = pkmnsim_getpkxstat(pkx, Stats::ATTACK);
        p_pkx->partyx_data.defense = pkmnsim_getpkxstat(pkx, Stats::DEFENSE);
        p_pkx->partyx_data.spatk = pkmnsim_getpkxstat(pkx, Stats::SPECIAL_ATTACK);
        p_pkx->partyx_data.spdef = pkmnsim_getpkxstat(pkx, Stats::SPECIAL_DEFENSE);
        p_pkx->partyx_data.speed = pkmnsim_getpkxstat(pkx, Stats::SPEED);
        
        SQLite::Database db(get_database_path().c_str());
        std::string pkxlevelsql = getpkmlevelsql(int(pkx->species),
                                                 int(pkx->exp));
        unsigned int level = int(db.execAndGet(pkxlevelsql.c_str()));
    }
    
    uint8_t pkmnsim_game_to_hometown(uint8_t game)
    {
        switch(game)
        {
            case Games::RUBY:
                return Hometowns::RUBY;

            case Games::SAPPHIRE:
                return Hometowns::SAPPHIRE;

            case Games::EMERALD:
                return Hometowns::EMERALD;

            case Games::FIRE_RED:
                return Hometowns::FIRE_RED;

            case Games::LEAF_GREEN:
                return Hometowns::LEAF_GREEN;

            case Games::COLOSSEUM:
            case Games::XD:
                return Hometowns::COLOSSEUM_XD;

            case Games::DIAMOND:
                return Hometowns::DIAMOND;

            case Games::PEARL:
                return Hometowns::PEARL;

            case Games::PLATINUM:
                return Hometowns::PLATINUM;

            case Games::HEART_GOLD:
                return Hometowns::HEART_GOLD;

            case Games::SOUL_SILVER:
                return Hometowns::SOUL_SILVER;

            case Games::BLACK:
                return Hometowns::BLACK;

            case Games::WHITE:
                return Hometowns::WHITE;

            case Games::BLACK2:
                return Hometowns::BLACK2;

            case Games::WHITE2:
                return Hometowns::WHITE2;

            default:
                return Hometowns::RUBY;
        }
    }

    uint8_t hometown_to_pkmnsim_game(uint8_t hometown)
    {
        switch(hometown)
        {
            case Hometowns::RUBY:
                return Games::RUBY;

            case Hometowns::SAPPHIRE:
                return Games::SAPPHIRE;

            case Hometowns::EMERALD:
                return Games::EMERALD;

            case Hometowns::FIRE_RED:
                return Games::FIRE_RED;

            case Hometowns::LEAF_GREEN:
                return Games::LEAF_GREEN;

            case Hometowns::COLOSSEUM_XD:
                return Games::XD;

            case Hometowns::DIAMOND:
                return Games::DIAMOND;

            case Hometowns::PEARL:
                return Games::PEARL;

            case Hometowns::PLATINUM:
                return Games::PLATINUM;

            case Hometowns::HEART_GOLD:
                return Games::HEART_GOLD;

            case Hometowns::SOUL_SILVER:
                return Games::SOUL_SILVER;

            case Hometowns::BLACK:
                return Games::BLACK;

            case Hometowns::WHITE:
                return Games::WHITE;

            case Hometowns::BLACK2:
                return Games::BLACK2;

            case Hometowns::WHITE2:
                return Games::WHITE2;

            default:
                return Games::RUBY;
        }
    }

    uint8_t pkmnsim_ball_to_game_ball(uint8_t ball)
    {
        switch(ball)
        {
            case PokeBalls::UNKNOWN:
                return ::Balls::pokeball_;

            case PokeBalls::POKE_BALL:
                return ::Balls::pokeball;

            case PokeBalls::GREAT_BALL:
                return ::Balls::greatball;

            case PokeBalls::ULTRA_BALL:
                return ::Balls::ultraball;

            case PokeBalls::MASTER_BALL:
                return ::Balls::masterball;

            case PokeBalls::SAFARI_BALL:
                return ::Balls::safariball;

            case PokeBalls::LEVEL_BALL:
                return ::Balls::levelball;

            case PokeBalls::LURE_BALL:
                return ::Balls::lureball;

            case PokeBalls::MOON_BALL:
                return ::Balls::moonball;

            case PokeBalls::FRIEND_BALL:
                return ::Balls::friendball;

            case PokeBalls::LOVE_BALL:
                return ::Balls::loveball;

            case PokeBalls::HEAVY_BALL:
                return ::Balls::heavyball;

            case PokeBalls::FAST_BALL:
                return ::Balls::fastball;

            case PokeBalls::SPORT_BALL:
                return ::Balls::sportball;

            case PokeBalls::PREMIER_BALL:
                return ::Balls::premierball;

            case PokeBalls::REPEAT_BALL:
                return ::Balls::repeatball;

            case PokeBalls::TIMER_BALL:
                return ::Balls::timerball;

            case PokeBalls::NEST_BALL:
                return ::Balls::nestball;

            case PokeBalls::NET_BALL:
                return ::Balls::netball;

            case PokeBalls::DIVE_BALL:
                return ::Balls::diveball;

            case PokeBalls::LUXURY_BALL:
                return ::Balls::luxuryball;

            case PokeBalls::HEAL_BALL:
                return ::Balls::healball;

            case PokeBalls::QUICK_BALL:
                return ::Balls::quickball;

            case PokeBalls::DUSK_BALL:
                return ::Balls::duskball;

            case PokeBalls::CHERISH_BALL:
                return ::Balls::cherishball;

            case PokeBalls::PARK_BALL:
                return ::Balls::pokeball; //This should never be set, this ball is for Pal Park

            case PokeBalls::DREAM_BALL:
                return ::Balls::dreamball;

            default:
                return ::Balls::pokeball;
        }
    }

    uint8_t game_ball_to_pkmnsim_ball(uint8_t game_ball)
    {
        switch(game_ball)
        {
            case ::Balls::pokeball_:
                return PokeBalls::UNKNOWN;

            case ::Balls::masterball:
                return PokeBalls::MASTER_BALL;

            case ::Balls::ultraball:
                return PokeBalls::ULTRA_BALL;

            case ::Balls::greatball:
                return PokeBalls::GREAT_BALL;

            case ::Balls::pokeball:
                return PokeBalls::POKE_BALL;

            case ::Balls::safariball:
                return PokeBalls::SAFARI_BALL;

            case ::Balls::netball:
                return PokeBalls::NET_BALL;

            case ::Balls::diveball:
                return PokeBalls::DIVE_BALL;

            case ::Balls::nestball:
                return PokeBalls::NEST_BALL;

            case ::Balls::repeatball:
                return PokeBalls::REPEAT_BALL;

            case ::Balls::timerball:
                return PokeBalls::TIMER_BALL;

            case ::Balls::luxuryball:
                return PokeBalls::LUXURY_BALL;

            case ::Balls::premierball:
                return PokeBalls::PREMIER_BALL;

            case ::Balls::duskball:
                return PokeBalls::DUSK_BALL;

            case ::Balls::healball:
                return PokeBalls::HEAL_BALL;

            case ::Balls::quickball:
                return PokeBalls::QUICK_BALL;

            case ::Balls::cherishball:
                return PokeBalls::CHERISH_BALL;

            case ::Balls::fastball:
                return PokeBalls::FAST_BALL;

            case ::Balls::levelball:
                return PokeBalls::LEVEL_BALL;

            case ::Balls::lureball:
                return PokeBalls::LURE_BALL;

            case ::Balls::heavyball:
                return PokeBalls::HEAVY_BALL;

            case ::Balls::loveball:
                return PokeBalls::LOVE_BALL;

            case ::Balls::friendball:
                return PokeBalls::FRIEND_BALL;

            case ::Balls::moonball:
                return PokeBalls::MOON_BALL;

            case ::Balls::sportball:
                return PokeBalls::SPORT_BALL;

            case ::Balls::dreamball:
                return PokeBalls::DREAM_BALL;

            default:
                return PokeBalls::POKE_BALL;
        }
    }
} /* namespace pkmnsim */
