/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_DATABASE_QUERIES_HPP
#define INCLUDED_PKMN_DATABASE_QUERIES_HPP

#include <string>

#include <pkmn/config.hpp>

namespace pkmn
{
    namespace database
    {
        unsigned int PKMN_API get_ability_id(const std::string &ability_name);
        std::string PKMN_API get_ability_name(const unsigned int ability_id);
        std::string PKMN_API get_ability_description(const unsigned int ability_id, const unsigned int version_id);
        std::string PKMN_API get_ability_description(const std::string &ability_name, const std::string &version_name);

        unsigned int PKMN_API get_egg_group_id(const std::string &egg_group_name);
        std::string PKMN_API get_egg_group_name(const unsigned int egg_group_id);

        unsigned int PKMN_API get_generation(const unsigned int version_id);
        unsigned int PKMN_API get_generation(const std::string &version_name);

        unsigned int PKMN_API get_item_id(const std::string &item_name);
        unsigned int PKMN_API get_item_id(const unsigned int game_index, const unsigned int version_id);
        unsigned int PKMN_API get_item_id(const unsigned int game_index, const std::string &version_name);
        std::string PKMN_API get_item_name(const unsigned int item_id);
        std::string PKMN_API get_item_name(const unsigned int game_index, const unsigned int version_id);
        std::string PKMN_API get_item_name(const unsigned int game_index, const std::string &version_name);
        unsigned int PKMN_API get_item_category_id(const unsigned int item_id);
        unsigned int PKMN_API get_item_category_id(const std::string &item_name);
        std::string PKMN_API get_item_category_name(const unsigned int item_id);
        std::string PKMN_API get_item_category_name(const std::string &item_name);
        std::string PKMN_API get_item_description(const unsigned int item_id, const unsigned int version_id);
        std::string PKMN_API get_item_description(const std::string &item_name, const std::string &version_name);
        unsigned int PKMN_API get_item_game_index(const unsigned int item_id, const unsigned int version_id);
        unsigned int PKMN_API get_item_game_index(const std::string &item_name, const std::string &version_name);

        unsigned int PKMN_API get_move_id(const std::string &move_name);
        std::string PKMN_API get_move_name(const unsigned int move_id);
        unsigned int PKMN_API get_move_damage_class_id(const unsigned int move_id);
        unsigned int PKMN_API get_move_damage_class_id(const std::string &move_name);
        std::string PKMN_API get_move_damage_class_name(const unsigned int move_id);
        std::string PKMN_API get_move_damage_class_name(const std::string &move_name);
        std::string PKMN_API get_move_description(const unsigned int move_id, const unsigned int version_id);
        std::string PKMN_API get_move_description(const std::string &move_name, const std::string &version_name);
        unsigned int PKMN_API get_move_pp(const unsigned int move_id);
        unsigned int PKMN_API get_move_pp(const std::string &move_name);

        unsigned int PKMN_API get_nature_id(const std::string &nature_name);
        std::string PKMN_API get_nature_name(const unsigned int nature_id);
        double PKMN_API get_nature_stat_mod(const unsigned int nature_id, const unsigned int stat_id);
        double PKMN_API get_nature_stat_mod(const std::string &nature_name, const std::string &stat_name);

        std::string PKMN_API get_pokedex_entry(const unsigned int species_id, const unsigned int version_id);
        std::string PKMN_API get_pokedex_entry(const std::string &species_name, const std::string &version_name);

        unsigned int PKMN_API get_pokemon_id(const unsigned int game_index, const unsigned int version_id);
        unsigned int PKMN_API get_pokemon_id(const unsigned int game_index, const std::string &version_name);
        unsigned int PKMN_API get_pokemon_id(const unsigned int form_id);
        unsigned int PKMN_API get_pokemon_game_index(const unsigned int pokemon_id, const unsigned int version_id);
        unsigned int PKMN_API get_pokemon_game_index(const unsigned int pokemon_id, const std::string &version_name);

        unsigned int PKMN_API get_species_id(const std::string &species_name);
        unsigned int PKMN_API get_species_id(const unsigned int pokemon_id);
        std::string PKMN_API get_species_name(const unsigned int species_id);

        unsigned int PKMN_API get_stat_id(const std::string &stat_name);
        std::string PKMN_API get_stat_name(const unsigned int stat_id);

        unsigned int PKMN_API get_type_id(const std::string &type_name);
        std::string PKMN_API get_type_name(const unsigned int type_id);

        unsigned int PKMN_API get_version_id(const std::string &version_name);
        std::string PKMN_API get_version_name(const unsigned int version_id);

        unsigned int PKMN_API get_version_group_id(const unsigned int version_id);
        unsigned int PKMN_API get_version_group_id(const std::string &version_name);
        std::string PKMN_API get_version_group_name(const unsigned int version_group_id);
    }
}

#endif /* INCLUDED_PKMN_DATABASE_QUERIES_HPP */
