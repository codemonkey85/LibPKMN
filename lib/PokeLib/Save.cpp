/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/pokelib/Save.hpp>

using namespace std;

namespace pkmnsim
{
    namespace PokeLib
    {
        Save::Save(::PokeLib::Save* save) {nativeSave = save;}

        Save::Save(string filename)
        {
            nativeSave = new ::PokeLib::Save(filename.c_str());
            nativeSave->parseRawSave();
        }

        Save::sptr Save::make(string filename) {return sptr(new Save(filename));}

        unsigned short Save::calcBlockAChecksum() {return nativeSave->calcBlockAChecksum();}
        unsigned short Save::calcBlockBChecksum() {return nativeSave->calcBlockBChecksum();}
        void Save::repairBlockAChecksum() {nativeSave->repairBlockAChecksum();}
        void Save::repairBlockBChecksum() {nativeSave->repairBlockBChecksum();}
        unsigned int Save::getSaveType() {return nativeSave->getSaveType();}
        unsigned int Save::getSaveFormat() {return nativeSave->getSaveFormat();}
        unsigned int Save::getSavePosition() {return nativeSave->getSavePosition();}
        Box::sptr Save::getBox(unsigned char n) const {return Box::make(nativeSave->getBox(n));}
        void Save::setBox(unsigned char n, Box::sptr b) {nativeSave->setBox(n, b->nativeBox);}
        Party::sptr Save::getParty() const {return Party::make(nativeSave->getParty());}
        void Save::setParty(Party::sptr p) {nativeSave->setParty(p->nativeParty);}
        Trainer::sptr Save::getTrainer() const {return Trainer::make(nativeSave->getTrainer());}
        void Save::setTrainer(Trainer::sptr p) {nativeSave->setTrainer(p->nativeTrainer);}

        void Save::setSavePosition(unsigned int p)
        {
            if(p > 1)
            {
                cerr << "SavePosition must be 0 or 1." << endl;
                exit(EXIT_FAILURE);
            }
            else nativeSave->setSavePosition((::PokeLib::SavePosition)(p));
        }
    }
}
