#ifndef BASE_PKMN_HPP
#define BASE_PKMN_HPP

#include <boost/shared_ptr.hpp>
#include <ciso646>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace pkmnsim
{

    /*!
     * Base Pokemon Class
     *
     * This class generates an object with all of the information for
     * a Pokemon, common to all Pokemon of this species. This information
     * includes name, Pokedex number, height, weight, etc.
     *
     * The information generated for a base Pokemon class is dependent on
     * the "identifier" parameter, which tells the program what to look for
     * in the database, as well as the "gen" parameter, which specifies the
     * generation from which to grab stats. This is important because certain
     * stats were added between generations (Special Attack/Defense, gender,
     * etc.), and some mechanics were completely changed between generations
     * 2-3. Therefore, a Gen 1 Pikachu will be very different than a
     * Gen 3 Pikachu.
     */
    class base_pkmn
    {
        public:

            typedef boost::shared_ptr<base_pkmn> sptr;

            /*!
             * Make a new base Pokemon.
             * \param identifier The Pokemon's identifier in the database
             * \param gen This generation's implementation of the Pokemon.
             * \return A new base Pokemon shared pointer
            */
            static sptr make(std::string identifier, int gen);

            /*!
             * Get a string with basic information on the Pokemon.
             * \return String with basic Pokemon info
             */
            virtual std::string get_info(void) = 0;

            /*!
             * Get a string with all information on the Pokemon.
             * \return String with all Pokemon info
             */
            virtual std::string get_info_verbose(void) = 0;

            /*!
             * Return Pokemon's display name (e.g. Bulbasaur).
             * \return String with display name
             */
            std::string get_display_name(void) {return display_name;}

            /*!
             * Return Pokemon's National Pokedex number (1-649, as of Gen 5).
             * \return Int with National Pokedex number
             */
            int get_nat_pokedex_num(void) {return nat_pokedex_num;}

            /*!
             * Return Pokemon's types.
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
             * Return Pokemon's height (in meters).
             * \return Int with Pokemon height
             */
            int get_height(void) {return height;}

            /*!
             * Return Pokemon's weight (in kilograms).
             * \return Int with Pokemon weight
             */
            int get_weight(void) {return weight;}

            /*!
             * Return Pokemon's base stats.
             * \return List of ints with base stats
             */
            virtual std::map<std::string,int> get_base_stats(void) = 0;

            /*!
             * Return Pokemon's EV yields.
             * In Gen 1-2, simply calls get_base_stats().
             * \return String with display name
             */
            virtual std::map<std::string,int> get_ev_yields(void) = 0;

            /*!
             * Return Pokemon's chance of being male.
             * NOTE: Will throw an error unless overridden.
             * \return Double with Pokemon's chance of being male
             */
            virtual double get_chance_male(void) {throw std::runtime_error("Not valid in this generation.");}

            /*!
             * Return Pokemon's chance of being female.
             * NOTE: Will throw an error unless overridden.
             * \return Double with Pokemon's chance of being female
             */
            virtual double get_chance_female(void) {throw std::runtime_error("Not valid in this generation.");}

            /*!
             * Return Pokemon's potential abilities.
             * NOTE: Will throw an error unless overriden.
             * \return List of strings with Pokemon's abilities
             */
            virtual std::string * get_abilities(void) {throw std::runtime_error("Not valid in this generation.");}

        protected:
            std::string display_name;
            int nat_pokedex_num;
            std::string species;
            std::string type1, type2;
            double height; //meters
            double weight; //kilograms
            int baseHP, baseATK, baseDEF, baseSPD; //Base stats
            int exp_yield;

    };
}
#endif
