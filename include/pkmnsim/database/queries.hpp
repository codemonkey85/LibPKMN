/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_DATABASE_QUERIES_HPP
#define INCLUDED_DATABASE_QUERIES_HPP

#include <string>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    namespace database
    {
        /*
         * Returns given string in a format to be used with database
         */
        std::string PKMNSIM_API to_database_format(std::string identifier);

        /*
         * Get the database ability ID associated with the specified name.
         */
        int PKMNSIM_API get_ability_id_from_name(std::string ability_name);

        /*
         * Get the ability name associated with the specified ID.
         */
        std::string PKMNSIM_API get_ability_name_from_id(int ability_id);

        /*
         * Get the database move ID associated with the specified name.
         */
        int PKMNSIM_API get_move_id_from_name(std::string move_name);

        /*
         * Get the move name associated with the specified ID.
         */
        std::string PKMNSIM_API get_move_name_from_id(int move_id);

        /*
         * Get the database nature ID associated with the specified name.
         */
        int PKMNSIM_API get_nature_id_from_name(std::string nature_name);

        /*
         * Get the nature name associated with the specified ID.
         */
        std::string PKMNSIM_API get_nature_name_from_id(int nature_id);

        /*
         * Get the Pokedex entry associated with the given species ID
         * and version enum.
         */
        std::string PKMNSIM_API get_pokedex_entry_from_species_id(int species_id, int version);

        /*
         * Get the Pokedex entry associated with the given species name
         * and version enum.
         */
        std::string PKMNSIM_API get_pokedex_entry_from_species_name(std::string species_name, int version);

        /*
         * Get the Pokemon species name associated with the specified ID.
         */
        std::string PKMNSIM_API get_species_name_from_id(int species_id);

        /*
         * Get the database type ID associated with the specified name.
         */
        int PKMNSIM_API get_type_id_from_name(std::string type_name);

        /*
         * Get the type name associated with the specified ID.
         */
        std::string PKMNSIM_API get_type_name_from_id(int type_id);
    }
}

#endif /* INCLUDED_DATABASE_QUERIES_HPP */
