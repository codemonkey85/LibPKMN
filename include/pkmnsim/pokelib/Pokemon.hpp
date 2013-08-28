/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_POKELIB_POKEMON_HPP
#define INCLUDED_PKMNSIM_POKELIB_POKEMON_HPP

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

#include <pokelib/pokelib.h>

namespace pkmnsim
{
    namespace PokeLib
    {
        class PKMNSIM_API Pokemon
        {
            public:

                typedef std::shared_ptr<Pokemon> sptr;

                //Class constructors (should never be called directly)
                Pokemon();
                Pokemon(::PokeLib::Pokemon* pokemon);

                static sptr make();
                static sptr make(::PokeLib::Pokemon* pokemon);

                /*
                 * There is no C++ wrapper for Python that allows direct
                 * manipulation of bitfields, so these get and set
                 * functions are necessary. The string to pass in is the
                 * name of the variable in the raw underlying type.
                 */
                unsigned int getAttribute(std::string attribute);
                void setAttribute(std::string attribute, unsigned int value);

                std::string getNickname() const;
                std::string getTrainerName() const;
                void updateBattleStats();
                void setNickname(std::string name);
                void setTrainerName(std::string name);
                unsigned char getLevel() const;
                unsigned int getExpToNextLevel() const;
                void setLevel(unsigned char level);
                unsigned char getGenderValue() const;
                unsigned char getNatureValue() const;
                unsigned int getPersonalityGender() const;
                unsigned char getHiddenPowerType() const;
                unsigned char getHiddenPowerAttack() const;
                bool isShiny() const;
                void setUnownForm(char f);

            private:
                ::PokeLib::Pokemon* nativePokemon;

                void exportValues();
                void importValues();

                dict<std::string, unsigned int> pRibbonSinnohA;
                dict<std::string, unsigned int> pRibbonSinnohB;
                dict<std::string, unsigned int> pRibbonHoenn;
                dict<std::string, unsigned int> pPKM;

                friend class Box;
                friend class Party;
        };
    }
}
#endif /* INCLUDED_PKMNSIM_POKELIB_POKEMON_HPP */
