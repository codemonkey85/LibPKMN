/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_POKELIB_BOX_HPP
#define INCLUDED_PKMNSIM_POKELIB_BOX_HPP

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
        class PKMNSIM_API Box
        {
            public:

                typedef std::shared_ptr<Box> sptr;

                //Class constructors (should never be called directly)
                Box();
                Box(::PokeLib::Box* box);

                static sptr make();
                static sptr make(::PokeLib::Box* box);

                std::string getName() const;
                void setName(std::string name);
                unsigned char getWallPaper() const;
                void setWallpaper(unsigned char numb);
                Pokemon::sptr getPokemon(unsigned char slot) const;
                void setPokemon(unsigned char slot, Pokemon::sptr pkm);
                void erasePokemon(unsigned char slot);
                unsigned char count();

            private:
                ::PokeLib::Box* nativeBox;

                friend class Save;
        };
    }
}
#endif /* INCLUDED_PKMNSIM_POKELIB_BOX_HPP */
