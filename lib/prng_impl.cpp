/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

#include <pkmn/types/prng.hpp>

#include "prng_impl.hpp"

namespace pkmn
{
    prng::sptr prng_impl::_gen3_instance = prng::sptr(new prng_impl(3));
    prng::sptr prng_impl::_gen4_instance = prng::sptr(new prng_impl(4));
    prng::sptr prng_impl::_gen5_instance = prng::sptr(new prng_impl(5));
    prng::sptr prng_impl::_gen6_instance = prng::sptr(new prng_impl(6));
    uint32_t prng_impl::_gen34_lcrng_seed = rand();
    uint64_t prng_impl::_gen56_lcrng_seed = rand();
    uint32_t prng_impl::_arng_seed = rand();
    unsigned int prng_impl::_mtrng_pos = 0;
    uint32_t prng_impl::_mtrng_seeds[];

    prng::sptr prng::get(unsigned int gen)
    {
        if(gen > 6) throw std::runtime_error("Generation must be 1-6!");
        else if(gen == 4) return prng_impl::_gen4_instance;
        else if(gen == 5) return prng_impl::_gen5_instance;
        else if(gen == 6) return prng_impl::_gen6_instance;
        else return prng_impl::_gen3_instance;
    }

    prng_impl::prng_impl(unsigned int gen): prng()
    {
        /*
         * If first index of MTRNG seed array is zero, assume it hasn't been
         * initialized.
         */
        if(_mtrng_seeds[0] == 0)
        {
            srand(time(NULL));
            for(size_t i = 0; i < 624; i++) _mtrng_seeds[i] = rand();
        }
    }

    uint64_t prng_impl::lcrng()
    {
        if(_generation < 5)
        {
            _gen34_lcrng_seed = (0x41C64E6D * _gen34_lcrng_seed) + 0x6073;
            return _gen34_lcrng_seed;
        }
        else
        {
            _gen56_lcrng_seed = (0x5D588B656C078965 * _gen56_lcrng_seed) + 0x0000000000269EC3;
            return _gen56_lcrng_seed;
        }
    }

    uint32_t prng_impl::arng()
    {
        _arng_seed = (0x6C078965 * _arng_seed) + 0x1;
        return _arng_seed;
    }

    uint32_t prng_impl::mtrng()
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
} /* namespace pkmn */
