/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef BASE_PKMN_HPP
#define BASE_PKMN_HPP

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <map>
#include "../../lib/sqlitecpp/SQLiteCPP.h"
#include <pkmnsim/base_move.hpp>
#include <pkmnsim/config.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace pkmnsim
{

    /*!
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

			base_pkmn() {};
			base_pkmn(std::string identifier, int gen, SQLite::Database *db, bool query_moves);
			
            /*!
             * Make a new base Pokémon.
             * \param identifier The Pokémon's identifier in the database
             * \param gen This generation's implementation of the Pokémon.
             * \param query_moves Choose whether or not to fill legal_moves vector.
             * \return A new base Pokémon shared pointer
             */
            static sptr make(std::string identifier, int gen, bool query_moves);

            /*!
             * Get a string with basic information on the Pokémon.
             * \return String with basic Pokémon info
             */
            virtual std::string get_info(void) = 0;

            /*!
             * Get a string with all information on the Pokémon.
             * \return String with all Pokémon info
             */
            virtual std::string get_info_verbose(void) = 0;

            /*!
             * Return Pokémon's display name (e.g. Bulbasaur).
             * \return String with display name
             */
            std::string get_display_name(void) {return display_name;}

            /*!
             * Return Pokémon's National Pokedex number (1-649, as of Gen 5).
             * \return Int with National Pokedex number
             */
            int get_nat_pokedex_num(void) {return nat_pokedex_num;}

            /*!
             * Return Pokémon's types.
             * \return List of strings with types
             */
            std::string * get_types(void)
            {
                std::string * types = new std::string[2];
                types[0] = type1;
                types[1] = type2;
                return types;
            }

            /*!
             * Return Pokémon's height (in meters).
             * \return Int with Pokémon height
             */
            int get_height(void) {return height;}

            /*!
             * Return Pokémon's weight (in kilograms).
             * \return Int with Pokémon weight
             */
            int get_weight(void) {return weight;}

            /*!
             * Return Pokémon's base stats.
             * \return List of ints with base stats
             */
            virtual std::map<std::string,int> get_base_stats(void) = 0;

            /*!
             * Return Pokémon's EV yields.
             * In Gen 1-2, simply calls get_base_stats().
             * \return String with display name
             */
            virtual std::map<std::string,int> get_ev_yields(void) = 0;

            /*!
             * Return Pokémon's chance of being male.
             * NOTE: Will throw an error unless overridden.
             * \return Double with Pokémon's chance of being male
             */
            virtual double get_chance_male(void) {throw std::runtime_error("Not valid in this generation.");}

            /*!
             * Return Pokémon's chance of being female.
             * NOTE: Will throw an error unless overridden.
             * \return Double with Pokémon's chance of being female
             */
            virtual double get_chance_female(void) {throw std::runtime_error("Not valid in this generation.");}

            /*!
             * Return Pokémon's potential abilities.
             * NOTE: Will throw an error unless overriden.
             * \return List of strings with Pokémon's abilities
             */
            virtual std::string * get_abilities(void) {throw std::runtime_error("Not valid in this generation.");}
			
			/*!
			 * If Pokémon can be evolved, returns vector of base_pkmn objects of all evolutions.
			 * \param evolution_vec A refernce to a vector in which to place list of Pokémon.
			 */
			void get_evolutions(std::vector<sptr>& evolution_vec);

			/*!
			 * If Pokémon is fully evolved, returns true. If it can evolve further, returns false.
			 * \return Bool showing if Pokémon is fully evolved
			 */
			bool is_fully_evolved();

            std::vector<base_move::sptr> get_legal_moves() {return legal_moves;}
            //std::vector<base_move::sptr> get_machine_moves() {return machine_moves;}
            //std::map<int, base_move::sptr> get_level_moves() {return level_moves;}

        protected:
            std::string display_name;
			std::string database_identifier;
            int pkmn_id, species_id; //Database values
			int from_gen;
            int nat_pokedex_num;
            std::string species;
            std::string type1, type2;
            double height; //meters
            double weight; //kilograms
            int baseHP, baseATK, baseDEF, baseSPD; //Base stats
            int exp_yield;
            std::vector<base_move::sptr> legal_moves; //All moves legally available
            //std::vector<base_move::sptr> machine_moves; //All moves taught by TM/HM
            //std::map<int, base_move::sptr> level_moves; //All moves learned by levelling up

            //Only used by spec_pkmn implementations
            friend int get_pkmn_id(sptr base);
            friend int get_species_id(sptr base);
    };

    /*!
     * User-friendly wrapper for base_pkmn::make
     * \param identifier The Pokémon's identifier in the database.
     * \param gen This generation's implementation of the Pokémon
     * \param query_moves Choose whether ot not to fill legal_moves vector.
     * \return A new base Pokémon shared pointer
     */
    base_pkmn::sptr PKMNSIM_API get_base_pkmn(std::string identifier, int gen, bool query_moves);

    /*!
     * Return a vector with all base Pokémon of specified type combo.
     * \param pkmn_vector A reference to a vector in which to place list of Pokémon.
     * \param type1 The type (or one of the types) of Pokémon to return
     * \param type2 The second type of the type combo of Pokémon to return
     * \param gen Only return Pokémon present in this generation.
     * \param lax If only one type specified, use all type combos with this type
     */
    void PKMNSIM_API get_pkmn_of_type(std::vector<base_pkmn::sptr>& pkmn_vector, std::string type1, std::string type2, int gen, bool lax);
}
#endif
