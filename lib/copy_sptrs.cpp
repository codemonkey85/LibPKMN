/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "copy_sptrs.hpp"

#include "bag_impl.hpp"
#include "base_pokemon_gen1impl.hpp"
#include "base_pokemon_gen2impl.hpp"
#include "base_pokemon_modernimpl.hpp"
#include "game_save_gen1impl.hpp"
#include "item_impl.hpp"
#include "item_berryimpl.hpp"
#include "item_machineimpl.hpp"
#include "move_impl.hpp"
#include "pocket_impl.hpp"
#include "team_pokemon_gen1impl.hpp"
#include "team_pokemon_gen2impl.hpp"
#include "team_pokemon_modernimpl.hpp"
#include "trainer_impl.hpp"

namespace pkmn
{
    bag::sptr copy_bag(bag::sptr in)
    {
        bag_impl actual = *pkmn::dynamic_pointer_cast<bag_impl>(in);
        return pkmn::dynamic_pointer_cast<bag>(pkmn::make_shared<bag_impl>(actual));
    }

    base_pokemon::sptr copy_base_pokemon(base_pokemon::sptr in)
    {
        switch(in->get_generation()) //Generation determines implementation class
        {
            case 1:
            {
                base_pokemon_gen1impl actual1 = *pkmn::dynamic_pointer_cast<base_pokemon_gen1impl>(in);
                return pkmn::make_shared<base_pokemon_gen1impl>(actual1);
            }

            case 2:
            {
                base_pokemon_gen2impl actual2 = *pkmn::dynamic_pointer_cast<base_pokemon_gen2impl>(in);
                return pkmn::make_shared<base_pokemon_gen2impl>(actual2);
            }

            default:
            {
                base_pokemon_modernimpl actual3 = *pkmn::dynamic_pointer_cast<base_pokemon_modernimpl>(in);
                return pkmn::make_shared<base_pokemon_modernimpl>(actual3);
            }
        }
    }

    item::sptr copy_item(item::sptr in)
    {
        unsigned int id = in->get_item_id();
        if((id >= Items::TM01 and id <= Items::HM08)
           or (id >= Items::TM93 and id <= Items::TM95)
           or (id >= Items::TM96 and id <= Items::TM100))
        {
            item_machineimpl actual_machine = *pkmn::dynamic_pointer_cast<item_machineimpl>(in);
            return pkmn::make_shared<item_machineimpl>(actual_machine);
        }
        else if((id >= Items::CHERI_BERRY and id <= Items::ROWAP_BERRY)
                or (id >= Items::BERRY and id <= Items::MYSTERYBERRY))
        {
            item_berryimpl actual_berry = *pkmn::dynamic_pointer_cast<item_berryimpl>(in);
            return pkmn::make_shared<item_berryimpl>(actual_berry);
        }
        else
        {
            item_impl actual = *pkmn::dynamic_pointer_cast<item_impl>(in);
            return pkmn::make_shared<item_impl>(actual);
        }
    }

    move::sptr copy_move(move::sptr in)
    {
        move_impl actual = *pkmn::dynamic_pointer_cast<move_impl>(in);
        return pkmn::make_shared<move_impl>(actual);
    }

    pocket::sptr copy_pocket(pocket::sptr in)
    {
        pocket_impl actual = *pkmn::dynamic_pointer_cast<pocket_impl>(in);
        return pkmn::make_shared<pocket_impl>(actual);
    }

    team_pokemon::sptr copy_team_pokemon(team_pokemon::sptr in)
    {
        switch(in->get_generation()) //Generation determines implementation class
        {
            case 1:
            {
                team_pokemon_gen1impl actual1 = *pkmn::dynamic_pointer_cast<team_pokemon_gen1impl>(in);
                return pkmn::make_shared<team_pokemon_gen1impl>(actual1);
            }

            case 2:
            {
                team_pokemon_gen2impl actual2 = *pkmn::dynamic_pointer_cast<team_pokemon_gen2impl>(in);
                return pkmn::make_shared<team_pokemon_gen2impl>(actual2);
            }

            default:
            {
                team_pokemon_modernimpl actual3 = *pkmn::dynamic_pointer_cast<team_pokemon_modernimpl>(in);
                return pkmn::make_shared<team_pokemon_modernimpl>(actual3);
            }
        }
    }

    trainer::sptr copy_trainer(trainer::sptr in)
    {
        trainer_impl actual = *pkmn::dynamic_pointer_cast<trainer_impl>(in);
        return pkmn::make_shared<trainer_impl>(actual);
    }
}
