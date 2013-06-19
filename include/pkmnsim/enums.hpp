/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_ENUMS_HPP
#define INCLUDED_PKMNSIM_ENUMS_HPP

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*
     * Globally used enums
     */

    namespace Genders
    {
        enum genders
        {
            MALE = 0,
            FEMALE = 1,
            GENDERLESS = 2
        };
    }

    namespace Move_Classes
    {
        enum move_classes
        {
            PHYSICAL = 1,
            SPECIAL = 2,
            EFFECT = 3
        };
    }

    namespace Statuses
    {
        enum nonvolatile_statuses
        {
            OK = 0, //No nonvolatile status
            BRN = 1, //Burn
            FRZ = 2, //Frozen
            PRZ = 3, //Paralysis
            PSN = 4, //Poison
            BPN = 5, //Bad Poison
            SLP = 6 //Asleep
        };
    }

}

#endif /* INCLUDED_PKMNSIM_ENUMS_HPP */
