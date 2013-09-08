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
        /*
         * Returns given string in a format to be used with the PKMNsim
         * database
         *
         * Parameters:
         *  - identifier: string to convert, usually a Pokemon or move name
         */
        std::string PKMNSIM_API to_database_format(std::string identifier);

        /*
         * Query the PKMNsim database for the ability ID associated with
         * the given name.
         *
         * Parameters:
         *  - ability_name: Name of ability whose ID to query
         */
        int PKMNSIM_API get_ability_id_from_name(std::string ability_name);

        /*
         * Query the PKMNsim database for the ability name associated with
         * the given ID.
         *
         * Parameters:
         *  - ability_id: ID of ability whose name to query
         */
        std::string PKMNSIM_API get_ability_name_from_id(int ability_id);

        /*
         * Query the PKMNsim database for the movs ID associated with
         * the given name.
         *
         * Parameters:
         *  - movs_name: Name of movs whose ID to query
         */
        int PKMNSIM_API get_move_id_from_name(std::string move_name);

        /*
         * Query the PKMNsim database for the move name associated with
         * the given ID.
         *
         * Parameters:
         *  - move_id: ID of move whose name to query
         */
        std::string PKMNSIM_API get_move_name_from_id(int move_id);

        /*
         * Query the PKMNsim database for the nature ID associated with
         * the given name.
         *
         * Parameters:
         *  - nature_name: Name of nature whose ID to query
         */
        int PKMNSIM_API get_nature_id_from_name(std::string nature_name);

        /*
         * Query the PKMNsim database for the nature name associated with
         * the given ID.
         *
         * Parameters:
         *  - nature_id: ID of nature whose name to query
         */
        std::string PKMNSIM_API get_nature_name_from_id(int nature_id);

        /*
         * Query the Pokedex entry associated with the given species ID
         * and version enum.
         *
         * Parameters:
         *  - species_id: ID of species whose Pokedex entry to query
         *  - version: enum entry of version ID whose game to grab entry from
         */
        std::string PKMNSIM_API get_pokedex_entry_from_species_id(int species_id, int version);

        /*
         * Query the Pokedex entry associated with the given species name
         * and version enum.
         *
         * Parameters:
         *  - species_name: name of species whose Pokedex entry to query
         *  - version: enum entry of version ID whose game to grab entry from
         */
        std::string PKMNSIM_API get_pokedex_entry_from_species_name(std::string species_name, int version);

        /*
         * Pokemon with different forms have different Pokemon ID's for each
         * form but have a common species ID. Specify a pokemon_id and get 
         * a species_id.
         *
         * Parameters:
         *  - pkmn_id: pokemon_id to query the database with
         */
        int PKMNSIM_API get_species_id_from_pokemon_id(int pkmn_id);

        /*
         * Query the PKMNsim database for the species ID associated with
         * the given name.
         *
         * Parameters:
         *  - species_id: name of species whose ID to query
         */
        int PKMNSIM_API get_species_id_from_name(std::string species_name);

        /*
         * Query the PKMNsim database for the species name associated with
         * the given ID.
         *
         * Parameters:
         *  - species_id: ID of species whose name to query
         */
        std::string PKMNSIM_API get_species_name_from_id(int species_id);

        /*
         * Query the PKMNsim database for the type ID associated with
         * the given name.
         *
         * Parameters:
         *  - type_name: Name of type whose ID to query
         */
        int PKMNSIM_API get_type_id_from_name(std::string type_name);

        /*
         * Query the PKMNsim database for the type name associated with
         * the given ID.
         *
         * Parameters:
         *  - type_id: ID of ability whose name to query
         */
        std::string PKMNSIM_API get_type_name_from_id(int type_id);
    }
}

#endif /* INCLUDED_PKMNSIM_DATABASE_QUERIES_HPP */
