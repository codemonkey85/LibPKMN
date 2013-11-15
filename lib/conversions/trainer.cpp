/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/database/queries.hpp>
#include <pkmnsim/types/pokemon_text.hpp>

#include "../library_bridge.hpp"
#include "items.hpp"
#include "pokemon.hpp"
#include "trainer.hpp"

using namespace std;

//Copied from PokeLib/lib/Trainer.cpp

uint8_t itemNum[3][8] = {
    {165,50,100,12,40,64,15,30},
    {165,50,100,12,40,64,15,30},
    {165,43,100,12,40,64,24,30},
};
uint16_t itemOffset[3][8] = {
    {0x624,0x8B8,0x980,0xB10,0xB40,0xBE0,0xCE0,0xD1C},
    {0x630,0x8C4,0x98C,0xB1C,0xB4C,0xBEC,0xCEC,0xD28},
    {0x644,0x8D8,0x9A4,0xB34,0xB64,0xC04,0xD04,0xD64},
};

enum {
    offsetName = 0,
    offsetTID, offsetSID, offsetMoney,
    offsetGender, offsetCountry,
    offsetBadges, offsetBadges2,
    offsetAvatar, offsetRival,
};

uint16_t tOffset[][3] = {
    {0x0064, 0x0068, 0x0064}, //TNAME (x8 16 bit)
    {0x0074, 0x0078, 0x0074}, //TID
    {0x0076, 0x007A, 0x0076}, //SID
    {0x0078, 0x007C, 0x0078}, //MONEY
    {0x007C, 0x0080, 0x007C}, //GENDER
    {0x007D, 0x0081, 0x007D}, //COUNTRY
    {0x007E, 0x0082, 0x007E}, //BADGES (HGSS Johto)
    {0x007E, 0x0082, 0x0083}, //BADGES 2 (HGSS Kanto, others mirror)
    {0x007F, 0x0083, 0x007F}, //Multiplayer Avatar
    {0x25A8, 0x27E8, 0x22D4}, //Rival Name (x8 16 bit)
};

namespace pkmnsim
{
    namespace conversions
    {
        trainer::sptr import_trainer_from_pokehack(pokehack_sptr parser, char* game_data)
        {
            unsigned int game_code = game_data[POKEHACK_GAME_CODE];
            unsigned int pkmnsim_game_id = 0;

            switch(game_code)
            {
                case 0:
                    pkmnsim_game_id = Games::RUBY; //TODO: distinguish between Ruby/Sapphire
                    break;

                case 1:
                    pkmnsim_game_id = Games::FIRE_RED; //TODO: distinguish between FR/LG
                    break;

                default:
                    pkmnsim_game_id = Games::EMERALD;
                    break;
            }

            pokemon_text trainer_name = string(parser->get_text(reinterpret_cast<unsigned char*>(&(game_data[POKEHACK_PLAYER_NAME])), false));
            bool trainer_is_female = game_data[POKEHACK_PLAYER_GENDER];
            unsigned int pkmnsim_gender;
            
            if(trainer_is_female) pkmnsim_gender = Genders::FEMALE;
            else pkmnsim_gender = Genders::MALE;

            trainer::sptr pkmnsim_trainer = trainer::make(pkmnsim_game_id, trainer_name, pkmnsim_gender);

            pkmnsim_trainer->set_id(*(reinterpret_cast<uint32_t*>(&game_data[POKEHACK_TRAINER_ID])));
            pkmnsim_trainer->set_money(0); //Currently unimplemented in Pokehack

            bag::sptr bag_from_pokehack = import_items_from_pokehack(reinterpret_cast<unsigned char*>(game_data));
            bag::sptr trainer_bag = pkmnsim_trainer->get_bag();
            trainer_bag = bag_from_pokehack;

            for(size_t i = 0; i < 6; i++)
            {
                if(parser->pokemon_growth[i]->species == 0) break;
                else
                {
                    team_pokemon::sptr t_pkmn = pokehack_pokemon_to_team_pokemon(parser->pokemon[i],
                                                                                 parser->pokemon_attacks[i],
                                                                                 parser->pokemon_effort[i],
                                                                                 parser->pokemon_misc[i],
                                                                                 parser->pokemon_growth[i]);
                    pkmnsim_trainer->set_pokemon(i+1, t_pkmn);
                }
            }

            return pkmnsim_trainer;
        }

        void export_trainer_to_pokehack(trainer::sptr pkmnsim_trainer, pokehack_sptr parser, char* game_data)
        {
            dict<char, int> pokehack_reverse_char_map = get_pokehack_reverse_char_map();

            string trainer_name = pkmnsim_trainer->get_name();
            for(int i = 0; i < 7; i++)
            {
                game_data[POKEHACK_PLAYER_NAME+i] = pokehack_reverse_char_map[trainer_name[i]];
            }
            game_data[POKEHACK_PLAYER_GENDER] = (pkmnsim_trainer->get_gender() == Genders::MALE) ? 0 : 1;

            switch(pkmnsim_trainer->get_game_id())
            {
                case Games::RUBY:
                case Games::SAPPHIRE:
                    game_data[POKEHACK_GAME_CODE] = 0;
                    break;

                case Games::FIRE_RED:
                case Games::LEAF_GREEN:
                    game_data[POKEHACK_GAME_CODE] = 1;
                    break;

                default:
                    break;
            }

            uint32_t* pokehack_id = reinterpret_cast<uint32_t*>(game_data[POKEHACK_TRAINER_ID]);
            *pokehack_id = pkmnsim_trainer->get_id();

            export_items_to_pokehack(pkmnsim_trainer->get_bag(), reinterpret_cast<unsigned char*>(game_data));

            pokemon_team_t party = pkmnsim_trainer->get_party();
            for(int i = 0; i < 6; i++)
            {
                if(party[i]->get_species_id()== Species::NONE) break;
                else
                {
                    team_pokemon_to_pokehack_pokemon(party[i],
                                                     parser->pokemon[i],
                                                     parser->pokemon_attacks[i],
                                                     parser->pokemon_effort[i],
                                                     parser->pokemon_misc[i],
                                                     parser->pokemon_growth[i]);
                }
            }
        }

        trainer::sptr import_trainer_from_pokelib(pokelib_sptr pokelib_save)
        {
            PokeLib::Trainer* pokelib_trainer = pokelib_save->getTrainer();

            unsigned int game_id;
            unsigned int save_type = pokelib_save->getSaveType();

            /*
             * The save type doesn't distinguish between D/P or HG/SS, but
             * Pokemon have a field that does. Parse the party to try and
             * find a Pokemon with the same ID as the trainer and get
             * the correct game ID from that. If no such Pokemon exists,
             * use the save type.
             */
            PokeLib::Party* pokelib_party = pokelib_save->getParty();
            uint16_t pokelib_public_id = pokelib_save->BlockA[tOffset[offsetTID][save_type]];
            uint16_t pokelib_secret_id = pokelib_save->BlockA[tOffset[offsetSID][save_type]];
            bool found = false;

            for(size_t i = 1; i < (pokelib_party->count()); i++)
            {
                PokeLib::Pokemon pokelib_pokemon = pokelib_party->getPokemon(i);

                if((pokelib_pokemon.pkm->pkm.ot_id == pokelib_public_id)
                    and (pokelib_pokemon.pkm->pkm.ot_sid == pokelib_secret_id))
                {
                    game_id = hometown_to_pkmnsim_game(pokelib_pokemon.pkm->pkm.hometown);
                    found = true;
                    break;
                }
            }
            if(not found)
            {
                switch(save_type)
                {
                    case PokeLib::DP:
                        game_id = Games::DIAMOND; //TODO: Distinguish between D/P
                        break;

                    case PokeLib::PLAT:
                        game_id = Games::PLATINUM;
                        break;

                    default:
                        game_id = Games::HEART_GOLD; //TODO: Distinguish between HG/SS
                        break;
                }
            }
            pokemon_text trainer_name = pokelib_trainer->getName();
            unsigned int gender = (pokelib_trainer->getFemale()) ? Genders::FEMALE : Genders::MALE;

            trainer::sptr pkmnsim_trainer = trainer::make(game_id, trainer_name, gender);

            pkmnsim_trainer->set_public_id(pokelib_public_id);
            pkmnsim_trainer->set_secret_id(pokelib_secret_id);
            pkmnsim_trainer->set_money(*(reinterpret_cast<uint32_t*>(&(pokelib_save->BlockA[tOffset[offsetMoney][save_type]]))));

            bag::sptr pkmnsim_bag = pkmnsim_trainer->get_bag();
            pkmnsim_bag = import_items_from_pokelib(*pokelib_trainer, game_id);

            for(size_t i = 1; i < (pokelib_party->count()); i++)
            {
                PokeLib::Pokemon pokelib_pokemon = pokelib_party->getPokemon(i);

                if(pokelib_pokemon.pkm->pkm.species == 0) break;
                else pkmnsim_trainer->set_pokemon(i, pokelib_pokemon_to_team_pokemon(pokelib_party->getPokemon(i)));
            }

            return pkmnsim_trainer;
        }

        void export_trainer_to_pokelib(trainer::sptr pkmnsim_trainer, pokelib_sptr pokelib_save)
        {
            PokeLib::Trainer* pokelib_trainer = pokelib_save->getTrainer();

            pokelib_trainer->setName(pkmnsim_trainer->get_name());
            pokelib_trainer->setFemale(pkmnsim_trainer->get_gender() == Genders::FEMALE);

            export_items_to_pokelib(pkmnsim_trainer->get_bag(), pokelib_trainer, pkmnsim_trainer->get_game_id());

            //PokeLib::Save::getTrainer returns new trainer
            pokelib_save->setTrainer(pokelib_trainer);

            PokeLib::Party* pokelib_party = pokelib_save->getParty();

            for(int i = 1; i <= 6; i++)
            {
                team_pokemon::sptr t_pkmn = pkmnsim_trainer->get_pokemon(i);
                if(t_pkmn->get_species_id() == Species::NONE) break;
                else pokelib_party->setPokemon(i, team_pokemon_to_pokelib_pokemon(t_pkmn));
            }
        }

        trainer::sptr import_trainer_from_pkmds_g5(pkmds_g5_sptr pkmds_save)
        {
            /*
             * PKMDS has no way of distinguishing between the different Gen 5
             * games, and trainer gender hasn't been reverse engineered. To
             * find each, we will parse the party to determine the relevant
             * information from Pokemon with matching ID's. If none are found,
             * use defaults.
             */
            party_obj* pkmds_party = &(pkmds_save->cur.party);
            uint16_t pkmds_public_id = pkmds_save->cur.tid;
            uint16_t pkmds_secret_id = pkmds_save->cur.sid;
            unsigned int game_id, gender;
            bool found = false;

            for(size_t i = 0; i < pkmds_party->size; i++)
            {
                pokemon_obj* pkmds_pokemon = &(pkmds_party->pokemon[i].pkm_data);

                if((pkmds_pokemon->tid == pkmds_public_id) and (pkmds_pokemon->sid == pkmds_secret_id))
                {
                    game_id = hometown_to_pkmnsim_game(int(pkmds_pokemon->hometown));
                    gender = (get_gen4_5_otgender((reinterpret_cast<uint8_t*>(&(pkmds_pokemon->ball)+1)))) ?
                             Genders::FEMALE : Genders::MALE;
                    found = true;
                    break;
                }
            }
            if(not found)
            {
                game_id = Games::BLACK2;
                gender = Genders::MALE;
            }

            pokemon_text pkmds_name = ::getsavtrainername(pkmds_save->cur);
            trainer::sptr pkmnsim_trainer = trainer::make(game_id, pkmds_name, gender);

            pkmnsim_trainer->set_public_id(pkmds_public_id);
            pkmnsim_trainer->set_secret_id(pkmds_secret_id);
            pkmnsim_trainer->set_money(0);
            
            bag::sptr pkmnsim_bag = pkmnsim_trainer->get_bag();
            pkmnsim_bag = import_items_from_pkmds_g5(&(pkmds_save->cur.bag), Games::BLACK2);

            for(size_t i = 0; i < pkmds_party->size; i++)
            {
                pkmnsim_trainer->set_pokemon(i+1, pkmds_pokemon_to_team_pokemon(&(pkmds_party->pokemon[i])));
            }

            return pkmnsim_trainer;
        }
        
        void export_trainer_to_pkmds_g5(trainer::sptr pkmnsim_trainer, pkmds_g5_sptr pkmds_save)
        {
            std::wstring trainer_name = pkmnsim_trainer->get_name();
        
            for(size_t i = 0; i < trainer_name.size(); i++)
            {
                pkmds_save->cur.trainername[i] = trainer_name[i];
            }
            
            export_items_to_pkmds_g5(pkmnsim_trainer->get_bag(), &(pkmds_save->cur.bag));
            
            for(size_t i = 1; i <= 6; i++)
            {
                team_pokemon::sptr t_pkmn = pkmnsim_trainer->get_pokemon(i);
            
                if(t_pkmn->get_species_id() == Species::NONE) break;
                else team_pokemon_to_pkmds_pokemon(t_pkmn, &(pkmds_save->cur.party.pokemon[i-1]));
            }
        }
    }
}
