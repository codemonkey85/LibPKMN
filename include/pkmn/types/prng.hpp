/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_PRNG_HPP
#define INCLUDED_PKMN_TYPES_PRNG_HPP

#include <cstdint>

#include <pkmn/config.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    /*!
     * This class represents an in-game pseudorandom number generator (PRNG). The algorithms used by this class, while
     * weak, match those used in the generation specified in the factory function's "gen" parameter.
     *
     * This class operates as a pseudo-singleton. Instead of a single instance existing, a single instance exists for
     * each of the six generations, and each one is accessed by passing the appropriate parameter into the factory
     * function, as shown below:
     * <pre>
     * pkmn::prng::sptr gen3_instance = pkmn::prng::get(3);
     * pkmn::prng::sptr gen4_instance1 = pkmn::prng::get(4);
     * pkmn::prng::sptr gen4_instance2 = pkmn::prng::get(4);
     * </pre>
     *
     * All algorithms taken from: <a href="http://bulbapedia.bulbagarden.net/wiki/Pseudorandom_number_generation_in_Pok%C3%A9mon">Bulbapedia</a>
     */
    class PKMN_API prng
    {
        public:

            typedef pkmn::shared_ptr<prng> sptr;

            /*!
             * This is the PRNG's factory function. 
             *
             * \param gen generation of desired PRNG
             * \return shared pointer to instance of specified generation's PRNG
             */
            static sptr get(unsigned int gen);

            /*!
             * Linear Congruential Random Number Generator (LCRNG)
             *
             * "In Generation III and Generation IV, the games use a 32 bit LCRNG.
             *
             *  ...
             *
             * This algorithm is used for the following:
             *  * Encrypting Pokémon data in the save file.
             *  * Generate the personality value of a wild encountered Pokémon.
             *  * Generate the individual values of a wild encountered Pokémon in Generation III and Generation IV.
             *  * Determining the species of a wild Pokémon.
             *  * Determining the Pokémon Lottery Corner lucky number.
             *  *Determining if an egg will be available at the Pokémon Day Care.
             *
             * In Generation V and Generation VI, the games instead use a 64 bit LCRNG..."
             *
             * Excerpt from: <a href="http://bulbapedia.bulbagarden.net/wiki/Pseudorandom_number_generation_in_Pok%C3%A9mon">Bulbapedia</a>
             *
             * \return next LCRNG value
             */
            virtual uint64_t lcrng() = 0;
            
            /*!
             * Alternative Pseudorandom Number Generator (ARNG)
             *
             * "The Generation IV games also use a different algorithm to alternate, reroll, or modify a previously randomly generated value...
             *
             * This algorithm is used for the following:
             *  * Shiny Pokémon Ranger Manaphy egg restriction.
             *  * Determining the species of a Pokémon swarm.
             *  * Determining the Pokémon in the Great Marsh.
             *  * Determining the Pokémon in the Trophy Garden.
             *  * Mystery Gift shininess restriction for Type 1 Wonder Card Pokémon (currently all released with variable Natures)."
             *
             * Excerpt from: <a href="http://bulbapedia.bulbagarden.net/wiki/Pseudorandom_number_generation_in_Pok%C3%A9mon">Bulbapedia</a>
             *
             * \return next ARNG value
             */
            virtual uint32_t arng() = 0;

            /*!
             * Mersenne Twister (MTRNG)
             *
             * "Found in Generation IV and later, the Mersenne Twister is another type of random number generator that produces 624 random numbers at once.
             * These random numbers are then stored and used when needed. When all 624 numbers are used, another set is generated.
             *
             * This algorithm is used for the following:
             *  * Egg PIDs in Generation IV.
             *  * Friendship Change Calculations.
             *  * Sprite Animations in Generation V.
             *  * Individual Values in Generation V.
             *  * AI Move decisions in Generation V."
             *
             * Excerpt from: <a href="http://bulbapedia.bulbagarden.net/wiki/Pseudorandom_number_generation_in_Pok%C3%A9mon">Bulbapedia</a>
             *
             * \return next MTRNG value
             */
            virtual uint32_t mtrng() = 0;

            virtual ~prng() {}; //Can this be avoided?
    
        protected:
        
            prng() {};
    };
}

#endif /* INCLUDED_PKMN_TYPES_PRNG_HPP */
