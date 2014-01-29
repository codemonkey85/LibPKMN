/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/cast.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/database/queries.hpp>

#include "copy_sptrs.hpp"

#include "bag_impl.hpp"
#include "base_pokemon_gen1impl.hpp"
#include "base_pokemon_gen2impl.hpp"
#include "base_pokemon_modernimpl.hpp"
#include "game_save_gen1impl.hpp"
#include "game_save_gen3impl.hpp"
#include "game_save_gen4impl.hpp"
#include "game_save_gen5impl.hpp"
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
    bag::sptr copy_bag(bag::sptr original)
    {
        bag_impl actual = *(boost::polymorphic_downcast<bag_impl*>(original.get()));
        return bag::sptr(&actual);
    }

    base_pokemon::sptr copy_base_pokemon(base_pokemon::sptr original)
    {
        switch(original->get_generation())
        {
            case 1:
            {
                base_pokemon_gen1impl actual1 = *(boost::polymorphic_downcast<base_pokemon_gen1impl*>(original.get()));
                return base_pokemon::sptr(&actual1);
            }
            
            case 2:
            {
                base_pokemon_gen2impl actual2 = *(boost::polymorphic_downcast<base_pokemon_gen2impl*>(original.get()));
                return base_pokemon::sptr(&actual2);
            }
            
            default:
            {
                base_pokemon_modernimpl actual3456 = *(boost::polymorphic_downcast<base_pokemon_modernimpl*>(original.get()));
                return base_pokemon::sptr(&actual3456);
            }
        }
    }

    game_save::sptr copy_game_save(game_save::sptr original)
    {
        switch(database::get_generation(original->get_game_id()))
        {
            case 1:
            {
                game_save_gen1impl actual1 = *(boost::polymorphic_downcast<game_save_gen1impl*>(original.get()));
                return game_save::sptr(&actual1);
            }
            
            case 3:
            {
                game_save_gen3impl actual3 = *(boost::polymorphic_downcast<game_save_gen3impl*>(original.get()));
                return game_save::sptr(&actual3);
            }
            
            case 4:
            {
                game_save_gen4impl actual4 = *(boost::polymorphic_downcast<game_save_gen4impl*>(original.get()));
                return game_save::sptr(&actual4);
            }
            
            default:
            {
                game_save_gen5impl actual5 = *(boost::polymorphic_downcast<game_save_gen5impl*>(original.get()));
                return game_save::sptr(&actual5);
            }
        }
    }

    item::sptr copy_item(item::sptr original)
    {
        unsigned int id = original->get_item_id();
    
        if((id >= Items::TM01 and id <= Items::HM08)
           or (id >= Items::TM93 and id <= Items::TM95)
           or (id >= Items::TM96 and id <= Items::TM100))
        {
            item_machineimpl actual_machine = *(boost::polymorphic_downcast<item_machineimpl*>(original.get()));
            return item::sptr(&actual_machine);
        }
        else if((id >= Items::CHERI_BERRY and id <= Items::ROWAP_BERRY)
                or (id >= Items::BERRY and id <= Items::MYSTERYBERRY))
        {
            item_berryimpl actual_berry = *(boost::polymorphic_downcast<item_berryimpl*>(original.get()));
            return item::sptr(&actual_berry);
        }
        else
        {
            item_impl actual = *(boost::polymorphic_downcast<item_impl*>(original.get()));
            return item::sptr(&actual);
        }
    }

    move::sptr copy_move(move::sptr original)
    {
        move_impl actual = *(boost::polymorphic_downcast<move_impl*>(original.get()));
        return move::sptr(&actual);
    }

    pocket::sptr copy_pocket(pocket::sptr original)
    {
        pocket_impl actual = *(boost::polymorphic_downcast<pocket_impl*>(original.get()));
        return pocket::sptr(&actual);
    }

    team_pokemon::sptr copy_team_pokemon(team_pokemon::sptr original)
    {
        switch(original->get_generation())
        {
            case 1:
            {
                team_pokemon_gen1impl actual1 = *(boost::polymorphic_downcast<team_pokemon_gen1impl*>(original.get()));
                return team_pokemon::sptr(&actual1);
            }
            
            case 2:
            {
                team_pokemon_gen2impl actual2 = *(boost::polymorphic_downcast<team_pokemon_gen2impl*>(original.get()));
                return team_pokemon::sptr(&actual2);
            }
            
            default:
            {
                team_pokemon_modernimpl actual3456 = *(boost::polymorphic_downcast<team_pokemon_modernimpl*>(original.get()));
                return team_pokemon::sptr(&actual3456);
            }
        }
    }

    trainer::sptr copy_trainer(trainer::sptr original)
    {
        trainer_impl actual = *(boost::polymorphic_downcast<trainer_impl*>(original.get()));
        return trainer::sptr(&actual);
    }
}