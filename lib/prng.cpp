/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <ctime>
#include <stdlib.h>

#include <pkmnsim/types/prng.hpp>

namespace pkmnsim
{
    uint32_t prng::lcrng_seed;
    uint64_t prng::gen5_lcrng_seed;
    uint32_t prng::arng_seed;
    unsigned int prng::mtrng_pos;
    uint32_t prng::mtrng_seeds[];

    prng::prng()
    {
        srand( time(NULL) );

        lcrng_seed = rand();
        gen5_lcrng_seed = rand();
        
        arng_seed = rand();

        for(int i = 0; i < 624; i++) mtrng_seeds[i] = rand();
        mtrng_pos = 0;
    }

    uint64_t prng::lcrng_next(uint32_t gen)
    {
        if(gen == 5)
        {
            uint64_t result = (0x5D588B656C078965 * gen5_lcrng_seed) + 0x0000000000269EC3;
            gen5_lcrng_seed = result;
            return result;
        }
        else
        {
            uint32_t result = (0x41C64E6D * lcrng_seed) + 0x6073;
            lcrng_seed = result;
            return result;
        }
    }

    uint32_t prng::arng_next()
    {
        uint32_t result = (0x6C078965 * arng_seed) + 0x1;
        arng_seed = result;

        return result;
    }

    uint32_t prng::mtrng_next()
    {
        uint32_t result = mtrng_seeds[mtrng_pos];

        if(mtrng_pos == 623)
        {
            for(int i = 0; i < 624; i++) mtrng_seeds[i] = rand();
            mtrng_pos = 0;
        }
        else mtrng_pos++;

        return result;
    }
} /* namespace pkmnsim */