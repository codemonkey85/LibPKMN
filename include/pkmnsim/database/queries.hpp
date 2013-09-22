/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_DATABASE_QUERIES_HPP
#define INCLUDED_PKMNSIM_DATABASE_QUERIES_HPP

#include <string>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    namespace database
    {
        std::string PKMNSIM_API to_database_format(std::string identifier);
     
        unsigned int PKMNSIM_API get_ability_id_from_name(std::string ability_name);
        std::string PKMNSIM_API get_ability_name_from_id(unsigned int ability_id);

        unsigned int PKMNSIM_API get_egg_group_id_from_name(std::string egg_group_name);
        std::string PKMNSIM_API get_egg_group_name_form_id(unsigned int egg_group_id);
        
        unsigned int PKMNSIM_API get_game_id_from_name(std::string game_name);
        std::string PKMNSIM_API get_game_name_from_id(unsigned int game_id);
        
        unsigned int PKMNSIM_API get_generation_from_game_id(unsigned int game_id);
        
        unsigned int PKMNSIM_API get_move_id_from_name(std::string move_name);
        std::string PKMNSIM_API get_move_name_from_id(unsigned int move_id);

        unsigned int PKMNSIM_API get_nature_id_from_name(std::string nature_name);
        std::string PKMNSIM_API get_nature_name_from_id(unsigned int nature_id);

        std::string PKMNSIM_API get_pokedex_entry_from_species_id(unsigned int species_id, int version);
        std::string PKMNSIM_API get_pokedex_entry_from_species_name(std::string species_name, int version);

        unsigned int PKMNSIM_API get_species_id_from_pokemon_id(unsigned int pkmn_id);
        unsigned int PKMNSIM_API get_species_id_from_name(std::string species_name);
        std::string PKMNSIM_API get_species_name_from_id(unsigned int species_id);

        unsigned int PKMNSIM_API get_type_id_from_name(std::string type_name);
        std::string PKMNSIM_API get_type_name_from_id(unsigned int type_id);
    }
}

#endif /* INCLUDED_PKMNSIM_DATABASE_QUERIES_HPP */
