/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_TYPES_PRNG_HPP
#define INCLUDED_PKMNSIM_TYPES_PRNG_HPP

#include <cstdint>

#include <pkmn/config.hpp>

namespace pkmnsim
{
    class PKMNSIM_API prng
    {
        public:
        
            prng();

            uint64_t lcrng_next(uint32_t gen);

            uint32_t arng_next();

            uint32_t mtrng_next();

        private:
        
            static uint32_t _lcrng_seed;
            static uint64_t _gen5_lcrng_seed;

            static uint32_t _arng_seed;

            static unsigned int _mtrng_pos;
            static uint32_t _mtrng_seeds[624];
    };
}

#endif /* INCLUDED_PKMNSIM_TYPES_PRNG_HPP */
