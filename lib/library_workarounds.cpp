/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <bitset> 

#include <boost/assign.hpp>

#include <pkmnsim/enums.hpp>

#include <pokehack/pokestructs.h>

#include "library_workarounds.hpp"

#define MAX_NICKNAME_LEN 10
#define MAX_TRAINER_NAME_LEN 7

using namespace std;

namespace pkmnsim
{
    /***********
     * Pokehack
     ***********/

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

    uint8_t pokehack_get_IV(uint32_t* IVint, uint8_t IV)
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
    
    void pokehack_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val)
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
    
    bool pokehack_get_ribbon(uint32_t* ribbonint, uint8_t ribbon)
    {
        bitset<32> ribbon_bitset = int(*ribbonint);
        return ribbon_bitset[ribbon];
    }
    
    void pokehack_set_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val)
    {
        bitset<32> ribbon_bitset = int(*ribbonint);
        ribbon_bitset[ribbon] = val;
        *ribbonint = ribbon_bitset.to_ulong();
    }
}