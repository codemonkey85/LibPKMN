/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string.h>

#include <pkmnsim/pokelib/Party.hpp>

using namespace std;

namespace pkmnsim
{
    namespace PokeLib
    {
        Party::Party()  {nativeParty = new ::PokeLib::Party();}
        Party::Party(::PokeLib::Party* party) {nativeParty = party;}

        Party::sptr Party::make() {return sptr(new Party);}
        Party::sptr Party::make(::PokeLib::Party* party) {return sptr(new Party(party));}

        Pokemon::sptr Party::getPokemon(unsigned char slot) const
        {
            ::PokeLib::Pokemon pkmn = nativeParty->getPokemon(slot);
            return Pokemon::make(&pkmn);
        }

        void Party::setPokemon(unsigned char slot, Pokemon::sptr pkmn) {nativeParty->setPokemon(slot, *(pkmn->nativePokemon));}
        void Party::erasePokemon(unsigned char slot) {nativeParty->erasePokemon(slot);}
        unsigned char Party::count() {return nativeParty->count();}
        void Party::sort() {nativeParty->sort();}
    }
}
