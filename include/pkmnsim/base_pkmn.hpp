/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_PKMN_HPP
#define INCLUDED_BASE_PKMN_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/dict.hpp>

#include "../../lib/sqlitecpp/SQLiteCPP.h"

namespace pkmnsim
{

    /*
     * Base Pokémon Class
     *
     * This class generates an object with all of the information for
     * a Pokémon, common to all Pokémon of this species. This information
     * includes name, Pokedex number, height, weight, etc.
     *
     * The information generated for a base Pokémon class is dependent on
     * the "identifier" parameter, which tells the program what to look for
     * in the database, as well as the "gen" parameter, which specifies the
     * generation from which to grab stats. This is important because certain
     * stats were added between generations (Special Attack/Defense, gender,
     * etc.), and some mechanics were completely changed between generations
     * 2-3. Therefore, a Gen 1 Pikachu will be very different than a
     * Gen 3 Pikachu.
     */
    class PKMNSIM_API base_pkmn
    {
        public:

            typedef boost::shared_ptr<base_pkmn> sptr;

            //Class Constructors (should never be called directly)
			base_pkmn(void) {};
			base_pkmn(std::string identifier, int gen, SQLite::Database *db, bool query_moves);
			
            /*
             * Returns a std::shared_ptr<base_pkmn> of specified Pokémon.
             * Verifies validity of Pokémon+generation before returning value.
             *
             * Parameters:
             *  - identifier: name of Pokémon
             *  - gen: generation (1-5) from which to use Pokémon
             *  - query_moves: add list of legal moves (slows performance)
             */
            static sptr make(std::string identifier, int gen, bool query_moves);

            /*
             * Returns a string with basic information on the Pokémon.
             * String contains name, National Pokédex number, types, and base stats.
             */
            virtual std::string get_info(void) = 0;

            /*
             * Returns a string with all information on the Pokémon.
             */
            virtual std::string get_info_verbose(void) = 0;

            /*
             * Returns Pokémon's display name (e.g. Bulbasaur).
             */
            std::string get_display_name(void);

            /*
             * Returns Pokémon's National Pokedex number (1-649, as of Gen 5).
             */
            int get_nat_pokedex_num(void);

            /*
             * Returns Pokémon's types.
             */
            dict<int, std::string> get_types(void);

            /*
             * Returns Pokémon's height (in meters).
             */
            double get_height(void);

            /*
             * Return Pokémon's weight (in kilograms).
             * \return Int with Pokémon weight
             */
            double get_weight(void);

            /*
             * Returns Pokémon's base stats.
             *
             * Query stats as follows:
             *  - HP: dict_name["HP"]
             *  - Attack: dict_name["ATK"]
             *  - Defense: dict_name["DEF"]
             *  - Speed: dict_name["SPD"]
             *  - Special: dict_name["SPCL"] (Gen 1 only)
             *  - Special Attack: dict_name["SATK"] (Gen 2-5 only)
             *  - Special Defense: dict_name["SDEF"] (Gen 2-5 only)
             */
            virtual dict<std::string, int> get_base_stats(void) = 0;

            /*
             * Returns Pokémon's effort value yields.
             * In Gen 1-2, simply calls get_base_stats().
             *
             * Query stats as follows:
             *  - HP: dict_name["HP"]
             *  - Attack: dict_name["ATK"]
             *  - Defense: dict_name["DEF"]
             *  - Speed: dict_name["SPD"]
             *  - Special: dict_name["SPCL"] (Gen 1 only)
             *  - Special Attack: dict_name["SATK"] (Gen 2-5 only)
             *  - Special Defense: dict_name["SDEF"] (Gen 2-5 only)
             */
            virtual dict<std::string, int> get_ev_yields(void) = 0;

            /*
             * Return Pokémon's chance of being male.
             * NOTE: Will throw an error if Pokémon is from Generation 1.
             */
            virtual double get_chance_male(void) {throw std::runtime_error("Not valid in this generation.");}

            /*
             * Return Pokémon's chance of being female.
             * NOTE: Will throw an error if Pokémon is from Generation 1.
             */
            virtual double get_chance_female(void) {throw std::runtime_error("Not valid in this generation.");}

            /*
             * Return Pokémon's potential abilities.
             * NOTE: Will throw an error if Pokémon is from Generations 1-2.
             */
            virtual dict<int, std::string> get_abilities(void) {throw std::runtime_error("Not valid in this generation.");}
			
			/*
			 * If Pokémon can be evolved, returns vector of base_pkmn objects of all evolutions.
             * If not, vector is empty.
             *
             * Parameters:
             *  - evolution_vec: Reference to a pointer in which to place Pokémon
			 */
			void get_evolutions(std::vector<sptr>& evolution_vec);

			/*
			 * If Pokémon is fully evolved, returns true. If it can evolve further, returns false.
			 */
			bool is_fully_evolved(void);

            /*
             * Returns vector of moves that this Pokémon can legally learn in this generation.
             * NOTE: Vector will be empty if base_pkmn was declared with query_moves = False
             */
            std::vector<base_move::sptr> get_legal_moves(void);

        protected:
            //Database values
			std::string database_identifier;
            int pkmn_id, species_id;

            std::string display_name;
			int from_gen;
            int nat_pokedex_num;
            std::string species;
            std::string type1, type2;
            double height; //meters
            double weight; //kilograms
            int baseHP, baseATK, baseDEF, baseSPD; //Base stats common to all generations
            int exp_yield;
            std::vector<base_move::sptr> legal_moves; //All moves legally available

            //Only used internally
            friend int get_pkmn_id(sptr base);
            friend int get_species_id(sptr base);
    };

    /*
     * Return a vector with all base Pokémon of specified type combo.
     *
     * Parameters:
     *  - pkmn_vector:  A reference to a vector in which to place list of Pokémon.
     *  - type1: The type (or one of the types) of Pokémon to return
     *  - type2: The second type of the type combo of Pokémon to return
     *  - gen: Only return Pokémon present in this generation.
     *  - lax: If only one type is specified, use all type combos with this type
     */
    void PKMNSIM_API get_pkmn_of_type(std::vector<base_pkmn::sptr>& pkmn_vector, std::string type1, std::string type2, int gen, bool lax);
}

#endif /* INCLUDED_BASE_PKMN_HPP */
