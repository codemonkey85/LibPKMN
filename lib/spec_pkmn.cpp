#include <iostream>
#include <string>
#include <pkmnsim/spec_pkmn.hpp>

using namespace std;

namespace pkmnsim
{
    spec_pkmn::sptr spec_pkmn::make(string identifier, int gen)
    {
        if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

        base_pkmn::sptr base = base_pkmn::make(identifier, gen);

        switch(gen)
        {
            case 1:
                return sptr(new spec_pkmn_gen1impl(base, base->get_display_name(), 100));

            case 2:
                return sptr(new spec_pkmn_gen2impl(base, base->get_display_name(), 100));

            default:
                return sptr(new spec_pkmn_gen3impl(base, base->get_display_name(), 100));
        };
    }
}
