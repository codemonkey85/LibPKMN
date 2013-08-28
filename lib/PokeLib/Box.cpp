/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string.h>

#include <pkmnsim/pokelib/Box.hpp>

using namespace std;

namespace pkmnsim
{
    namespace PokeLib
    {
        Box::Box()  {nativeBox = new ::PokeLib::Box();}
        Box::Box(::PokeLib::Box* box) {nativeBox = box;}

        Box::sptr Box::make() {return sptr(new Box);}
        Box::sptr Box::make(::PokeLib::Box* box) {return sptr(new Box(box));}

        string Box::getName() const
        {
            ::PokeLib::widetext nameWide = nativeBox->getName();
            char tmp[32];
            memset(tmp,0,32);
            wcstombs(tmp, nameWide.c_str(), 32);
            return string(tmp);
        }

        void Box::setName(string name) {nativeBox->setName(::PokeLib::PokeText::toWidetext(name.c_str(), name.size()));}
        unsigned char Box::getWallPaper() const {return nativeBox->getWallpaper();}
        void Box::setWallpaper(unsigned char numb) {nativeBox->setWallpaper(numb);}

        Pokemon::sptr Box::getPokemon(unsigned char slot) const
        {
            ::PokeLib::Pokemon pkmn = nativeBox->getPokemon(slot);
            return Pokemon::make(&pkmn);
        }

        void Box::setPokemon(unsigned char slot, Pokemon::sptr pkmn) {nativeBox->setPokemon(slot, *(pkmn->nativePokemon));}
        void Box::erasePokemon(unsigned char slot) {nativeBox->erasePokemon(slot);}
        unsigned char Box::count() {return nativeBox->count();}
    }
}
