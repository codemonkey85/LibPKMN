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
     
        unsigned int PKMNSIM_API get_ability_id(std::string ability_name);
        std::string PKMNSIM_API get_ability_name(unsigned int ability_id);

        unsigned int PKMNSIM_API get_damage_class(unsigned int type_id);
        
        unsigned int PKMNSIM_API get_egg_group_id(std::string egg_group_name);
        std::string PKMNSIM_API get_egg_group_name(unsigned int egg_group_id);
        
        unsigned int PKMNSIM_API get_game_id(std::string game_name);
        std::string PKMNSIM_API get_game_name(unsigned int game_id);
        
        unsigned int PKMNSIM_API get_generation(unsigned int game_id);

        unsigned int PKMNSIM_API get_item_index(unsigned int item_id, unsigned int version);
        unsigned int PKMNSIM_API get_item_index(std::string item_name, unsigned int version);
        
        std::string PKMNSIM_API get_item_description(unsigned int item_id, unsigned int version);
        std::string PKMNSIM_API get_item_description(std::string name, unsigned int version);
        
        unsigned int PKMNSIM_API get_item_id(std::string item_name);
        unsigned int PKMNSIM_API get_item_id(unsigned int item_index, unsigned int game);
        std::string PKMNSIM_API get_item_name(unsigned int item_id);

        unsigned int PKMNSIM_API get_item_category(unsigned int item_id);
        unsigned int PKMNSIM_API get_item_category(std::string item_name);
        
        std::string PKMNSIM_API get_move_description(unsigned int move_id, unsigned int version);
        std::string PKMNSIM_API get_move_description(std::string move_name, unsigned int version);
        
        unsigned int PKMNSIM_API get_move_id(std::string move_name);
        std::string PKMNSIM_API get_move_name(unsigned int move_id);

        double PKMNSIM_API get_nature_stat_effect(unsigned int nature_id, unsigned int stat);
        double PKMNSIM_API get_nature_stat_effect(std::string nature_name, unsigned int stat);
        
        unsigned int PKMNSIM_API get_nature_id(std::string nature_name);
        std::string PKMNSIM_API get_nature_name(unsigned int nature_id);

        std::string PKMNSIM_API get_pokedex_entry(unsigned int species_id, unsigned int version);
        std::string PKMNSIM_API get_pokedex_entry(std::string species_name, unsigned int version);

        unsigned int PKMNSIM_API get_species_id(unsigned int pkmn_id);
        unsigned int PKMNSIM_API get_species_id(std::string species_name);
        std::string PKMNSIM_API get_species_name(unsigned int species_id);

        unsigned int PKMNSIM_API get_type_id(std::string type_name);
        std::string PKMNSIM_API get_type_name(unsigned int type_id);
        
        unsigned int PKMNSIM_API get_version_group(unsigned int version_id);
    }
}

#endif /* INCLUDED_PKMNSIM_DATABASE_QUERIES_HPP */
