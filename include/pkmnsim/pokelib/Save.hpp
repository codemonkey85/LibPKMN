/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_POKELIB_SAVE_HPP
#define INCLUDED_PKMNSIM_POKELIB_SAVE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <stdexcept>
#include <string>

#include <pkmnsim/config.hpp>
#include <pkmnsim/pokelib/Box.hpp>
#include <pkmnsim/pokelib/Party.hpp>
#include <pkmnsim/pokelib/Trainer.hpp>

#include <pokelib/pokelib.h>

namespace pkmnsim
{
    namespace PokeLib
    {
        class PKMNSIM_API Save
        {
            public:

                typedef std::shared_ptr<Save> sptr;

                //Class constructors (should never be called directly)
                Save() {};
                Save(::PokeLib::Save* save);
                Save(std::string filename);

                static sptr make(std::string filename);

                unsigned short calcBlockAChecksum();
                unsigned short calcBlockBChecksum();
                void repairBlockAChecksum();
                void repairBlockBChecksum();
                unsigned int getSaveType();
                unsigned int getSaveFormat();
                unsigned int getSavePosition();
                void setSavePosition(unsigned int p);
                Box::sptr getBox(unsigned char n) const;
                void setBox(unsigned char n, Box::sptr b);
                Party::sptr getParty() const;
                void setParty(Party::sptr p);
                Trainer::sptr getTrainer() const;
                void setTrainer(Trainer::sptr p);

            private:
                ::PokeLib::Save* nativeSave;

                friend class Trainer;
        };
    }
}
#endif /* INCLUDED_PKMNSIM_POKELIB_SAVE_HPP */
