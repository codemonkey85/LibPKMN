/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_BASE_PKMN_HPP
#define INCLUDED_PKMNSIM_BASE_PKMN_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/dict.hpp>

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
    class PKMNSIM_API base_pokemon
    {
        public:

            typedef std::shared_ptr<base_pokemon> sptr;

            //Class Constructors (should never be called directly)
			base_pokemon(void) {};
			base_pokemon(std::string identifier, int game);
			
            /*
             * Returns a std::shared_ptr<base_pokemon> of specified Pokémon.
             * Verifies validity of Pokémon+generation before returning value.
             *
             * Parameters:
             *  - identifier: name of Pokémon
             *  - gen: generation (1-5) from which to use Pokémon
             *  - query_moves: add list of legal moves (slows performance)
             */
            static sptr make(std::string identifier, int game);

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
            std::string get_species_name(void);

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
			 * If Pokémon can be evolved, returns vector of base_pokemon objects of all evolutions.
             * If not, vector is empty.
             *
             * Parameters:
             *  - evolution_vec: Reference to a pointer in which to place Pokémon
			 */
			void get_evolutions(std::vector<sptr> &evolution_vec);

			/*
			 * If Pokémon is fully evolved, returns true. If it can evolve further, returns false.
			 */
			bool is_fully_evolved(void);

            /*
             * Get game ID from which stats were derived.
             */
            int get_game_id(void);
            
            /*
             * Get generation from which stats were derived.
             */
            int get_generation(void);

            /*
             * Returns bool corresponding to whether or not this Pokemon has gender differences.
             */
            virtual bool has_gender_differences(void) = 0;

            /*
             * Returns pokemon.id from pkmnsim.db
             */
            int get_pokemon_id(void);
            
            /*
             * Returns pokemon_species.id from pkmnsim.db
             */
            int get_species_id(void);
            
            /*
             * Get path of icon on filesystem, using given gender.
             *
             * Parameters:
             *  - is_male: use a male icon
             */
            virtual std::string get_icon_path(bool is_male) = 0;
            
            /*
             * Get one of up to four sprite paths, using given parameters.
             *
             * Parameters:
             *  - is_male: use a male sprite
             *  - is_shiny: use the shiny sprite
             */
            virtual std::string get_sprite_path(bool is_male, bool is_shiny) = 0;

            /*
             * Sets base to form specified by enum int.
             *
             * Parameters:
             *  - form: enum int representing form
             */
            void set_form(int form);

            /*
             * Sets base to form specified by string.
             *
             * Parameters:
             *  - form: string representing form
             */
            void set_form(std::string form);

            /*
             * Update stats based on specified ID.
             *
             * Parameters:
             *  - id: Pokemon ID to base repairs on
             */
            void repair(int id);

        protected:
            //Database values
			std::string database_identifier;
            int from_gen, pkmn_id, species_id, type1_id, type2_id;
            
            int from_game;
            std::string game_string, images_game_string;
            std::string male_icon_path, female_icon_path;
            std::string male_sprite_path, female_sprite_path;
            std::string male_shiny_sprite_path, female_shiny_sprite_path;

    };

    //Related typedefs
    typedef std::vector<base_pokemon::sptr> b_pkmn_vec_t;
    typedef vla<base_pokemon::sptr> b_pkmn_vla_t;

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
    void PKMNSIM_API get_pkmn_of_type(b_pkmn_vec_t &pkmn_vector, std::string type1, std::string type2, int gen, bool lax);

    //Pass-as-value get_pkmn_of_type for SWIG
    b_pkmn_vec_t PKMNSIM_API get_pkmn_list_of_type(std::string type1, std::string type2, int gen, bool lax);
}

#endif /* INCLUDED_PKMNSIM_BASE_PKMN_HPP */
