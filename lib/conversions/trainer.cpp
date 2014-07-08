/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <pkmn/enums.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/pokemon_text.hpp>

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

namespace pkmn
{
    namespace conversions
    {
        trainer::sptr import_gen1_trainer(rpokesav_gen1_sptr sav)
        {
            pokemon_text trainer_name = sav->get_trainer_name();

            /*
             * Generation I has no way to distinguish between games, so just
             * use Yellow. There aren't enough differences to make a difference.
             */
            trainer::sptr libpkmn_trainer = trainer::make(Games::YELLOW,
                                                          sav->get_trainer_name(),
                                                          Genders::MALE);

            libpkmn_trainer->set_id(sav->get_trainer_id());
            libpkmn_trainer->set_money(sav->get_money());
            std::array<rpokesav::gen1_item_t,20> rpokesav_bag = sav->get_bag_items();

            import_gen1_bag(rpokesav_bag, libpkmn_trainer->get_bag(),
                            sav->get_bag_item_count());

            std::array<rpokesav::gen1_pokemon,6> rpokesav_team = sav->get_team();
            for(size_t i = 0; i < sav->get_team_size(); i++)
            {
                libpkmn_trainer->set_pokemon(i+1, import_gen1_pokemon(rpokesav_team[i]));
            }

            return libpkmn_trainer;
        }

        trainer::sptr import_gen3_trainer(pokehack_sptr parser, char* game_data)
        {
            unsigned int game_code = game_data[POKEHACK_GAME_CODE];
            unsigned int libpkmn_game_id = 0;

            switch(game_code)
            {
                case 0:
                    libpkmn_game_id = Games::RUBY; //TODO: distinguish between Ruby/Sapphire
                    break;

                case 1:
                    libpkmn_game_id = Games::FIRE_RED; //TODO: distinguish between FR/LG
                    break;

                default:
                    libpkmn_game_id = Games::EMERALD;
                    break;
            }

            pokemon_text trainer_name = string(parser->get_text(reinterpret_cast<unsigned char*>(&(game_data[POKEHACK_PLAYER_NAME])), false));
            bool trainer_is_female = game_data[POKEHACK_PLAYER_GENDER];
            unsigned int libpkmn_gender;

            if(trainer_is_female) libpkmn_gender = Genders::FEMALE;
            else libpkmn_gender = Genders::MALE;

            trainer::sptr libpkmn_trainer = trainer::make(libpkmn_game_id, trainer_name, libpkmn_gender);

            libpkmn_trainer->set_id(*(reinterpret_cast<uint32_t*>(&game_data[POKEHACK_TRAINER_ID])));
            libpkmn_trainer->set_money(0); //Currently unimplemented in Pokehack

            import_gen3_items(libpkmn_trainer->get_bag(), reinterpret_cast<unsigned char*>(game_data));

            for(size_t i = 0; i < 6; i++)
            {
                if(parser->pokemon_growth[i]->species == 0) break;
                else
                {
                    pokehack_args_t args;
                    args.b_pkmn_t = parser->pokemon[i];
                    args.pkmn_a_t = parser->pokemon_attacks[i];
                    args.pkmn_e_t = parser->pokemon_effort[i];
                    args.pkmn_m_t = parser->pokemon_misc[i];
                    args.pkmn_g_t = parser->pokemon_growth[i];

                    team_pokemon::sptr t_pkmn = import_gen3_pokemon(args);
                    libpkmn_trainer->set_pokemon(i+1, t_pkmn);
                }
            }

            return libpkmn_trainer;
        }

        void export_gen3_trainer(trainer::sptr libpkmn_trainer, pokehack_sptr parser, char* game_data)
        {
            string trainer_name = libpkmn_trainer->get_name();
            for(int i = 0; i < 7; i++)
            {
                game_data[POKEHACK_PLAYER_NAME+i] = pokehack_reverse_char_map[trainer_name[i]];
            }
            game_data[POKEHACK_PLAYER_GENDER] = (libpkmn_trainer->get_gender().std_string() == "Male") ? 0 : 1;

            switch(libpkmn_trainer->get_game_id())
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
            *pokehack_id = libpkmn_trainer->get_id();

            export_gen3_items(libpkmn_trainer->get_bag(), reinterpret_cast<unsigned char*>(game_data));

            pokemon_team_t party;
            libpkmn_trainer->get_party(party);
            for(int i = 0; i < 6; i++)
            {
                if(party[i]->get_species_id()== Species::NONE) break;
                else
                {
                    pokehack_args_t args;
                    args.b_pkmn_t = parser->pokemon[i];
                    args.pkmn_a_t = parser->pokemon_attacks[i];
                    args.pkmn_e_t = parser->pokemon_effort[i];
                    args.pkmn_m_t = parser->pokemon_misc[i];
                    args.pkmn_g_t = parser->pokemon_growth[i];

                    export_gen3_pokemon(party[i], args);
                }
            }
        }

        trainer::sptr import_gen4_trainer(pokelib_sptr pokelib_save)
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
            uint16_t pokelib_public_id = pokelib_trainer->getPID();
            uint16_t pokelib_secret_id = pokelib_trainer->getSID();
            bool found = false;

            for(size_t i = 1; i <= (pokelib_party->count()); i++)
            {
                PokeLib::Pokemon pokelib_pokemon = pokelib_party->getPokemon(i);

                if((pokelib_pokemon.pkm->pkm.ot_id == pokelib_public_id)
                    and (pokelib_pokemon.pkm->pkm.ot_sid == pokelib_secret_id))
                {
                    game_id = hometown_to_libpkmn_game(pokelib_pokemon.pkm->pkm.hometown);
                    found = true;
                    break;
                }
            }
            if(not found)
            {
                switch(save_type)
                {
                    case PokeLib::DP:
                        game_id = Games::DIAMOND;
                        break;

                    case PokeLib::PLAT:
                        game_id = Games::PLATINUM;
                        break;

                    default:
                        game_id = Games::HEART_GOLD;
                        break;
                }
            }
            pokemon_text trainer_name = pokelib_trainer->getName();
            unsigned int gender = (pokelib_trainer->getFemale()) ? Genders::FEMALE : Genders::MALE;

            trainer::sptr libpkmn_trainer = trainer::make(game_id, trainer_name, gender);

            libpkmn_trainer->set_public_id(pokelib_public_id);
            libpkmn_trainer->set_secret_id(pokelib_secret_id);
            libpkmn_trainer->set_money(pokelib_trainer->getMoney());

            import_gen4_items(libpkmn_trainer->get_bag(), *pokelib_trainer);

            for(size_t i = 1; i <= (unsigned int)(pokelib_party->count()); i++)
            {
                PokeLib::Pokemon pokelib_pokemon = pokelib_party->getPokemon(i);

                if(pokelib_pokemon.pkm->pkm.species == 0) break;
                else libpkmn_trainer->set_pokemon(i, import_gen4_pokemon(pokelib_pokemon));
            }

            return libpkmn_trainer;
        }

        void export_gen4_trainer(trainer::sptr libpkmn_trainer, pokelib_sptr pokelib_save)
        {
            PokeLib::Trainer* pokelib_trainer = pokelib_save->getTrainer();

            pokelib_trainer->setName(libpkmn_trainer->get_name());
            pokelib_trainer->setFemale(libpkmn_trainer->get_gender().std_string() == "Female");

            export_gen4_items(libpkmn_trainer->get_bag(), pokelib_trainer);

            //PokeLib::Save::getTrainer returns new trainer
            pokelib_save->setTrainer(pokelib_trainer);

            PokeLib::Party* pokelib_party = pokelib_save->getParty();

            for(int i = 1; i <= 6; i++)
            {
                team_pokemon::sptr t_pkmn = libpkmn_trainer->get_pokemon(i);
                if(t_pkmn->get_species_id() == Species::NONE) break;
                else pokelib_party->setPokemon(i, export_gen4_pokemon(t_pkmn));
            }
        }

        trainer::sptr import_gen5_trainer(pkmds_g5_sptr pkmds_save)
        {
            /*
             * PKMDS has no way of distinguishing between the different Gen 5
             * games, and trainer gender hasn't been reverse engineered. To
             * find each, we will parse the party to determine the relevant
             * information from Pokemon with matching ID's. If none are found,
             * use defaults.
             */
            std::array<party_pkm, 6> pkmds_party = pkmds_save->cur.party.pokemon;
            uint16_t pkmds_public_id = pkmds_save->cur.tid;
            uint16_t pkmds_secret_id = pkmds_save->cur.sid;
            unsigned int game_id, gender;
            bool found = false;

            for(size_t i = 0; i < pkmds_party.size(); i++)
            {
                pokemon_obj* pkmds_pokemon = &(pkmds_party[i]);

                if((pkmds_pokemon->tid == pkmds_public_id) and (pkmds_pokemon->sid == pkmds_secret_id))
                {
                    game_id = hometown_to_libpkmn_game(int(pkmds_pokemon->hometown));
                    gender = (get_gen_456_otgender((reinterpret_cast<uint8_t*>(&(pkmds_pokemon->ball)+1)))) ?
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
            trainer::sptr libpkmn_trainer = trainer::make(game_id, pkmds_name, gender);

            libpkmn_trainer->set_public_id(pkmds_public_id);
            libpkmn_trainer->set_secret_id(pkmds_secret_id);
            libpkmn_trainer->set_money(0);

            import_gen5_items(libpkmn_trainer->get_bag(), &(pkmds_save->cur.bag));

            for(size_t i = 0; i < pkmds_party.size(); i++)
            {
                ::decryptpkm(&pkmds_party[i]);
                libpkmn_trainer->set_pokemon(i+1, import_gen5_pokemon(&(pkmds_party[i])));
                ::encryptpkm(&pkmds_party[i]);
            }

            return libpkmn_trainer;
        }

        void export_gen5_trainer(trainer::sptr libpkmn_trainer, pkmds_g5_sptr pkmds_save)
        {
            std::wstring trainer_name = libpkmn_trainer->get_name();

            for(size_t i = 0; i < trainer_name.size(); i++)
            {
                pkmds_save->cur.trainername[i] = trainer_name[i];
            }

            export_gen5_items(libpkmn_trainer->get_bag(), &(pkmds_save->cur.bag));

            for(size_t i = 1; i <= 6; i++)
            {
                team_pokemon::sptr t_pkmn = libpkmn_trainer->get_pokemon(i);

                if(t_pkmn->get_species_id() == Species::NONE) break;
                else export_gen5_pokemon(t_pkmn, &(pkmds_save->cur.party.pokemon[i-1]));
            }
        }
    } /* namespace conversions */
} /* namespace pkmn */
