/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "item_berryimpl.hpp"
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmnsim
{
    item_berryimpl::item_berryimpl(unsigned int id, unsigned int game): item_impl(id, game)
    {
        //Be forgiving and convert between Gen 2 and Gen 3-5 berries with same effect
        switch(database::get_generation(game))
        {
            case 2:
                if(id >= Items::CHERI_BERRY and id <= Items::ROWAP_BERRY) gen345_to_gen2();
                break;
            case 3:
            case 4:
            case 5:
                if(id >= Items::BERRY and id <= Items::MYSTERYBERRY) gen2_to_gen345();
                
            default:
                item_id = Items::INVALID;
        }
        
        category_id = database::get_item_category(id);
        game_index = database::get_item_index(item_id, game_id);
        description = database::get_item_description(item_id, game_id);
        item_name = database::get_item_name(id);
    }
    
    void item_berryimpl::gen2_to_gen345()
    {
        switch(item_id)
        {
            case Items::BERRY:
                item_id = Items::ORAN_BERRY;
                break;
                
            case Items::GOLD_BERRY:
                item_id = Items::SITRUS_BERRY;
                break;
                
            case Items::PRZCUREBERRY:
                item_id = Items::CHERI_BERRY;
                break;
                
            case Items::PSNCUREBERRY:
                item_id = Items::PECHA_BERRY;
                break;
                
            case Items::BITTER_BERRY:
                item_id = Items::PERSIM_BERRY;
                break;
                
            case Items::BURNT_BERRY:
                item_id = Items::ASPEAR_BERRY;
                break;
                
            case Items::ICE_BERRY:
                item_id = Items::RAWST_BERRY;
                break;
            
            case Items::MINT_BERRY:
                item_id = Items::CHESTO_BERRY;
                break;
            
            case Items::MIRACLEBERRY:
                item_id = Items::LUM_BERRY;
                break;
                
            case Items::MYSTERYBERRY:
                item_id = Items::LEPPA_BERRY;
                break;
            
            default: //This should never happen
                item_id = Items::INVALID;
                break;
        }
    }
    
    void item_berryimpl::gen345_to_gen2()
    {
        switch(item_id)
        {
            case Items::ORAN_BERRY:
                item_id = Items::BERRY;
                break;
                
            case Items::SITRUS_BERRY:
            item_id = Items::GOLD_BERRY;
                break;
                
            case Items::CHERI_BERRY:
                item_id = Items::PRZCUREBERRY;
                break;
            
            case Items::PECHA_BERRY:
                item_id = Items::PSNCUREBERRY;
                break;
            
            case Items::PERSIM_BERRY:
                item_id = Items::BITTER_BERRY;
                break;
            
            case Items::ASPEAR_BERRY:
                item_id = Items::BURNT_BERRY;
                break;
            
            case Items::RAWST_BERRY:
                item_id = Items::ICE_BERRY;
                break;
            
            case Items::CHESTO_BERRY:
                item_id = Items::MINT_BERRY;
                break;
            
            case Items::LUM_BERRY:
                item_id = Items::MIRACLEBERRY;
                break;
                
            case Items::LEPPA_BERRY:
                item_id = Items::MYSTERYBERRY;
            
            default:
                item_id = Items::INVALID;
                break;
        }
    }
}
