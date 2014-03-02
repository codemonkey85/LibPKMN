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
    class PKMN_API prng
    {
        public:

            typedef pkmn::shared_ptr<prng> sptr;
            static sptr get(unsigned int gen);

            virtual uint64_t lcrng() = 0;
            virtual uint32_t arng() = 0;
            virtual uint32_t mtrng() = 0;

            virtual ~prng() {}; //Can this be avoided?
    
        protected:
        
            prng() {};
    };
}

#endif /* INCLUDED_PKMN_TYPES_PRNG_HPP */
