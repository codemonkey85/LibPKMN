#include <iostream>
#include <string>
#include <time.h>
#include <pkmnsim/spec_pkmn.hpp>

//Random values
spec_pkmn::spec_pkmn(base_pkmn b, int lvl)
{
    //Random IV's
    srand ( time(NULL) );
    iv

    base = b;
    level = lvl;
}

/*
 * IV to HP (round down)
 * HP = ((ivHP + 2(base.bHP) + 0.25(base.evHP) + 100)(level))/100 + 10
 *
 * HP to ivHP (round up)
 * ivHP = (100(HP - 10))/level - 2(base.bHP) - 0.25(base.evHP) - 100
 */

/*
 * IV to other value (ATK as example) (round down)
 * ATK = ((ivATK + 2(base.bATK) = 0.25(evATK))/100) + 5
 *
 * Other value to IV (ivATK as example) (round up)
 * ivATK = ((ATK/nature_mod)-5)(100/level) - 2(base.bATK) - 0.25(base.evATK)
 */
