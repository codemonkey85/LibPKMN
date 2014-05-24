/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PRNG_IMPL_HPP
#define INCLUDED_PRNG_IMPL_HPP

#include <pkmn/types/prng.hpp>

namespace pkmn
{
    class prng_impl: public prng
    {
        public:

            prng_impl(unsigned int gen);
            ~prng_impl() {};

            uint64_t lcrng();
            uint32_t arng();
            uint32_t mtrng();

            static prng::sptr _gen3_instance;
            static prng::sptr _gen4_instance;
            static prng::sptr _gen5_instance;
            static prng::sptr _gen6_instance;

        private:

            unsigned int _generation;

            static uint32_t _gen34_lcrng_seed;
            static uint64_t _gen56_lcrng_seed;
            static uint32_t _arng_seed;
            static unsigned int _mtrng_pos;
            static uint32_t _mtrng_nums[624];
    };
}

#endif /* INCLUDED_PRNG_IMPL_HPP */
