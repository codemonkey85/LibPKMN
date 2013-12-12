/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <ctime>
#include <cstdlib>

#include <pkmnsim/types/prng.hpp>

namespace pkmnsim
{
    uint32_t prng::_lcrng_seed;
    uint64_t prng::_gen5_lcrng_seed;
    uint32_t prng::_arng_seed;
    unsigned int prng::_mtrng_pos;
    uint32_t prng::_mtrng_seeds[];

    prng::prng()
    {
        srand( time(NULL) );

        _lcrng_seed = rand();
        _gen5_lcrng_seed = rand();
        
        _arng_seed = rand();

        for(int i = 0; i < 624; i++) _mtrng_seeds[i] = rand();
        _mtrng_pos = 0;
    }

    uint64_t prng::lcrng_next(uint32_t gen)
    {
        if(gen == 5)
        {
            uint64_t result = (0x5D588B656C078965 * _gen5_lcrng_seed) + 0x0000000000269EC3;
            _gen5_lcrng_seed = result;
            return result;
        }
        else
        {
            uint32_t result = (0x41C64E6D * _lcrng_seed) + 0x6073;
            _lcrng_seed = result;
            return result;
        }
    }

    uint32_t prng::arng_next()
    {
        uint32_t result = (0x6C078965 * _arng_seed) + 0x1;
        _arng_seed = result;
        return result;
    }

    uint32_t prng::mtrng_next()
    {
        uint32_t result = _mtrng_seeds[_mtrng_pos];

        if(_mtrng_pos == 623)
        {
            for(int i = 0; i < 624; i++) _mtrng_seeds[i] = rand();
            _mtrng_pos = 0;
        }
        else _mtrng_pos++;
        return result;
    }
} /* namespace pkmnsim */
