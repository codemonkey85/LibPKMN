/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string.h>

#include <pkmnsim/pokelib/Trainer.hpp>

#include <pokelib/pokelib.h>

using namespace std;

namespace pkmnsim
{
    namespace PokeLib
    {
        Trainer::Trainer(::PokeLib::Trainer* save)
        {
            nativeTrainer = save;
            importBadges();
        }

        Trainer::Trainer(::PokeLib::Save* save)
        {
            nativeTrainer = new ::PokeLib::Trainer(save);
            importBadges();
        }

        Trainer::sptr Trainer::make() {return sptr(new Trainer());}
        Trainer::sptr Trainer::make(::PokeLib::Trainer* trainer) {return sptr(new Trainer(trainer));}

        vector<bool> Trainer::getBadges() {return mainBadges;}
        vector<bool> Trainer::getAltBadges() {return altBadges;}

        void Trainer::setBadges(vector<bool> b)
        {
            if(b.size() == 8)
            {
                mainBadges = b;
                exportBadges();
            }
            else
            {
                cerr << "Invalid vector given." << endl;
                exit(EXIT_FAILURE);
            }
        }

        void Trainer::setAltBadges(vector<bool> b)
        {
            if(b.size() == 8)
            {
                altBadges = b;
                exportBadges();
            }
            else
            {
                cerr << "Invalid vector given." << endl;
                exit(EXIT_FAILURE);
            }
        }

        string Trainer::getName()
        {
            ::PokeLib::widetext nameWide = nativeTrainer->getName();
            char tmp[32];
            memset(tmp,0,32);
            wcstombs(tmp, nameWide.c_str(), 32);
            return string(tmp);
        }

        void Trainer::setName(string text)
        {
            nativeTrainer->setName(::PokeLib::PokeText::toWidetext(text.c_str(), text.size()));
        }

        string Trainer::getRivalName()
        {
            ::PokeLib::widetext nameWide = nativeTrainer->getRivalName();
            char tmp[32];
            memset(tmp,0,32);
            wcstombs(tmp, nameWide.c_str(), 32);
            return string(tmp);
        }

        void Trainer::setRivalName(string text)
        {
            nativeTrainer->setRivalName(::PokeLib::PokeText::toWidetext(text.c_str(), text.size()));
        }

        bool Trainer::getFemale() {return nativeTrainer->getFemale();}
        void Trainer::setFemale(bool female) {nativeTrainer->setFemale(female);}

        unsigned char Trainer::getPocketSize(unsigned int pocket) const
        {
            return nativeTrainer->getPocketSize(::PokeLib::ItemPocket(pocket));
        }

        unsigned char Trainer::getPocketMaxSize(unsigned int pocket) const
        {
            return nativeTrainer->getPocketMaxSize(::PokeLib::ItemPocket(pocket));
        }

        pair<unsigned short, unsigned short> Trainer::getItem(unsigned int pocket, unsigned char slot) const
        {
            ::PokeLib::BagItem bagitem = nativeTrainer->getItem(::PokeLib::ItemPocket(pocket), slot);
            pair<unsigned short, unsigned short> item_pair;
            item_pair.first = bagitem.item;
            item_pair.second = bagitem.count;
            return item_pair;
        }

        void Trainer::setItem(unsigned int pocket, unsigned char slot, pair<unsigned short, unsigned short> item_pair)
        {
            ::PokeLib::BagItem bagitem;
            bagitem.item = item_pair.first;
            bagitem.count = item_pair.second;
            nativeTrainer->setItem(::PokeLib::ItemPocket(pocket), slot, bagitem);
        }

        void Trainer::eraseItem(unsigned int pocket, unsigned char slot)
        {
            nativeTrainer->eraseItem(::PokeLib::ItemPocket(pocket), slot);
        }

        void Trainer::sortPocket(unsigned int pocket)
        {
            nativeTrainer->sortPocket(::PokeLib::ItemPocket(pocket));
        }

        void Trainer::exportBadges()
        {
            nativeTrainer->badgeMain.badge_1 = mainBadges[0];
            nativeTrainer->badgeMain.badge_2 = mainBadges[1];
            nativeTrainer->badgeMain.badge_3 = mainBadges[2];
            nativeTrainer->badgeMain.badge_4 = mainBadges[3];
            nativeTrainer->badgeMain.badge_5 = mainBadges[4];
            nativeTrainer->badgeMain.badge_6 = mainBadges[5];
            nativeTrainer->badgeMain.badge_7 = mainBadges[6];
            nativeTrainer->badgeMain.badge_8 = mainBadges[7];

            nativeTrainer->badgeAlt.badge_1 = altBadges[0];
            nativeTrainer->badgeAlt.badge_2 = altBadges[1];
            nativeTrainer->badgeAlt.badge_3 = altBadges[2];
            nativeTrainer->badgeAlt.badge_4 = altBadges[3];
            nativeTrainer->badgeAlt.badge_5 = altBadges[4];
            nativeTrainer->badgeAlt.badge_6 = altBadges[5];
            nativeTrainer->badgeAlt.badge_7 = altBadges[6];
            nativeTrainer->badgeAlt.badge_8 = altBadges[7];
        }

        void Trainer::importBadges()
        {
            mainBadges.clear();
            mainBadges.push_back(nativeTrainer->badgeMain.badge_1);
            mainBadges.push_back(nativeTrainer->badgeMain.badge_2);
            mainBadges.push_back(nativeTrainer->badgeMain.badge_3);
            mainBadges.push_back(nativeTrainer->badgeMain.badge_4);
            mainBadges.push_back(nativeTrainer->badgeMain.badge_5);
            mainBadges.push_back(nativeTrainer->badgeMain.badge_6);
            mainBadges.push_back(nativeTrainer->badgeMain.badge_7);
            mainBadges.push_back(nativeTrainer->badgeMain.badge_8);

            altBadges.clear();
            altBadges.push_back(nativeTrainer->badgeAlt.badge_1);
            altBadges.push_back(nativeTrainer->badgeAlt.badge_2);
            altBadges.push_back(nativeTrainer->badgeAlt.badge_3);
            altBadges.push_back(nativeTrainer->badgeAlt.badge_4);
            altBadges.push_back(nativeTrainer->badgeAlt.badge_5);
            altBadges.push_back(nativeTrainer->badgeAlt.badge_6);
            altBadges.push_back(nativeTrainer->badgeAlt.badge_7);
            altBadges.push_back(nativeTrainer->badgeAlt.badge_8);
        }
    }
}
