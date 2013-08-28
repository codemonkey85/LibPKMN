/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_POKELIB_TRAINER_HPP
#define INCLUDED_PKMNSIM_POKELIB_TRAINER_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <pkmnsim/config.hpp>

#include <pokelib/pokelib.h>

namespace pkmnsim
{
    namespace PokeLib
    {
        class PKMNSIM_API Trainer
        {
            public:

                typedef std::shared_ptr<Trainer> sptr;

                //Class constructors (should never be called directly)
                Trainer() {};
                Trainer(::PokeLib::Trainer* trainer);
                Trainer(::PokeLib::Save* save);

                static sptr make();
                static sptr make(::PokeLib::Trainer* trainer);

                std::vector<bool> getBadges();
                std::vector<bool> getAltBadges();
                void setBadges(std::vector<bool> b);
                void setAltBadges(std::vector<bool> b);

                std::string getName();
                void setName(std::string text);
                std::string getRivalName();
                void setRivalName(std::string text);
                bool getFemale();
                void setFemale(bool female);
                unsigned char getPocketSize(unsigned int pocket) const;
                unsigned char getPocketMaxSize(unsigned int pocket) const;
                std::pair<unsigned short, unsigned short> getItem(unsigned int pocket, unsigned char slot) const;
                void setItem(unsigned int pocket, unsigned char slot, std::pair<unsigned short, unsigned short>);
                void eraseItem(unsigned int pocket, unsigned char slot);
                void sortPocket(unsigned int pocket);

            private:
                ::PokeLib::Trainer* nativeTrainer;

                void exportBadges();
                void importBadges();

                std::vector<bool> mainBadges;
                std::vector<bool> altBadges;

                friend class Save;
        };
    }
}
#endif /* INCLUDED_PKMNSIM_POKELIB_TRAINER_HPP */
