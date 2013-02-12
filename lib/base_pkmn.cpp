#include <iostream>
#include <string>
#include "base_pkmn_gen1impl.hpp"
#include "base_pkmn_gen2impl.hpp"
#include "base_pkmn_gen3impl.hpp"
#include <pkmnsim/base_pkmn.hpp>

using namespace std;

namespace pkmnsim
{
    base_pkmn::sptr base_pkmn::make(string identifier, int gen)
    {
        transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);

        if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

        switch(gen)
        {
            case 1:
                return sptr(new base_pkmn_gen1impl(identifier));

            case 2:
                return sptr(new base_pkmn_gen2impl(identifier));

            default:
                return sptr(new base_pkmn_gen3impl(identifier, gen));
        }
    }
}
