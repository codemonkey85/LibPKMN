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

    namespace Forms
    {
        namespace Pichu
        {
            enum forms
            {
                NORMAL = 0,
                SPIKY_EARED = 1
            };
        }

        namespace Unown
        {
            enum forms
            {
                A = 1,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
                EXCLAMATION,QUESTION
            };
        }

        namespace Castform
        {
            enum forms
            {
                NORMAL = 0,
                SUNNY = 1,
                RAINY = 2,
                SNOWY = 3
            };
        }

        namespace Deoxys
        {
            enum forms
            {
                NORMAL = 0,
                ATTACK = 1,
                DEFENSE = 2,
                SPEED = 3
            };
        }

        namespace Burmy
        {
            enum forms
            {
                PLANT_CLOAK = 0,
                SANDY_CLOAK = 1,
                TRASH_CLOAK = 2
            };
        }

        namespace Wormadam
        {
            enum forms
            {
                PLANT_CLOAK = 0,
                SANDY_CLOAK = 1,
                TRASH_CLOAK = 2
            };
        }

        namespace Cherrim
        {
            enum forms
            {
                OVERCAST = 0,
                SUNSHINE = 1
            };
        }

        namespace Shellos
        {
            enum forms
            {
                WEST_SEA = 0,
                EAST_SEA = 1
            };
        }

        namespace Gastrodon
        {
            enum forms
            {
                WEST_SEA = 0,
                EAST_SEA = 1
            };
        }

        namespace Rotom
        {
            enum forms
            {
                NORMAL = 0,
                HEAT = 1,
                WASH = 2,
                FROST = 3,
                FAN = 4,
                MOW = 5
            };
        }

        namespace Giratina
        {
            enum forms
            {
                ALTERED = 0,
                ORIGIN = 1
            };
        }

        namespace Shaymin
        {
            enum forms
            {
                LAND = 0,
                SKY = 1
            };
        }

        namespace Arceus
        {
            enum forms
            {
                NORMAL = 0,
                FIRE = 1,
                WATER = 2,
                ELECTRIC = 3,
                GRASS = 4,
                ICE = 5,
                FIGHTING = 6,
                POISON = 7,
                GROUND = 8,
                FLYING = 9,
                PSYCHIC = 10,
                BUG = 11,
                ROCK = 12,
                GHOST = 13,
                DRAGON = 14,
                DARK = 15,
                STEEL = 16
            };
        }

        namespace Basculin
        {
            enum forms
            {
                RED_STRIPED = 0,
                BLUE_STRIPED = 1
            };
        }

        namespace Darmanitan
        {
            enum forms
            {
                STANDARD = 0,
                ZEN = 1
            };
        }

        namespace Deerling
        {
            enum forms
            {
                SPRING = 0,
                SUMMER = 1,
                AUTUMN = 2,
                WINTER = 3
            };
        }

        namespace Sawsbuck
        {
            enum forms
            {
                SPRING = 0,
                SUMMER = 1,
                AUTUMN = 2,
                WINTER = 3
            };
        }

        namespace Tornadus
        {
            enum forms
            {
                INCARNATE = 0,
                THERIAN = 1
            };
        }

        namespace Thundurus
        {
            enum forms
            {
                INCARNATE = 0,
                THERIAN = 1
            };
        }

        namespace Landorus
        {
            enum forms
            {
                INCARNATE = 0,
                THERIAN = 1
            };
        }

        namespace Kyurem
        {
            enum forms
            {
                NORMAL = 0,
                BLACK = 1,
                WHITE = 2
            };
        }

        namespace Keldeo
        {
            enum forms
            {
                ORDINARY = 0,
                RESOLUTE = 1
            };
        }

        namespace Meloetta
        {
            enum forms
            {
                ARIA = 0,
                PIROUETTE = 1
            };
        }

        namespace Genesect
        {
            enum forms
            {
                NORMAL = 0,
                SHOCK_DRIVE = 1,
                BURN_DRIVE = 2,
                CHILL_DRIVE = 3,
                DOUSE_DRIVE = 4
            };
        }
    }

    namespace Move_Classes
    {
        enum move_classes
        {
            NON_DAMAGING = 1,
            PHYSICAL = 2,
            SPECIAL = 3
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

    namespace Games
    {
        enum games
        {
            RED = 1,
            BLUE,
            YELLOW,
            GOLD,
            SILVER,
            CRYSTAL,
            RUBY,
            SAPPHIRE,
            EMERALD,
            FIRE_RED,
            LEAF_GREEN,
            DIAMOND,
            PEARL,
            PLATINUM,
            HEART_GOLD,
            SOUL_SILVER,
            BLACK,
            WHITE,
            COLOSSEUM,
            XD,
            BLACK2,
            WHITE2,
            STADIUM,
            STADIUM2
        };
    }
}

#endif /* INCLUDED_PKMNSIM_ENUMS_HPP */
