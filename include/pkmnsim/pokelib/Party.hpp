/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_POKELIB_PARTY_HPP
#define INCLUDED_PKMNSIM_POKELIB_PARTY_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <stdexcept>
#include <string>

#include <pkmnsim/config.hpp>
#include <pkmnsim/dict.hpp>
#include <pkmnsim/pokelib/Pokemon.hpp>

#include <pokelib/pokelib.h>

namespace pkmnsim
{
    namespace PokeLib
    {
        class PKMNSIM_API Party
        {
            public:

                typedef std::shared_ptr<Party> sptr;

                //Class constructors (should never be called directly)
                Party();
                Party(::PokeLib::Party* pokemon);

                static sptr make();
                static sptr make(::PokeLib::Party* pokemon);

                Pokemon::sptr getPokemon(unsigned char slot) const;
                void setPokemon(unsigned char slot, Pokemon::sptr pkm);
                void erasePokemon(unsigned char slot);
                unsigned char count();
                void sort();

            private:
                ::PokeLib::Party* nativeParty;

                friend class Save;
        };
    }
}
#endif /* INCLUDED_PKMNSIM_POKELIB_PARTY_HPP */
