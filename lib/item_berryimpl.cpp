/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "item_berryimpl.hpp"
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

using namespace std;

namespace pkmn
{
    item_berryimpl::item_berryimpl(unsigned int id, unsigned int game): item_impl(id, game)
    {
        //Be forgiving and convert between Gen 2 and Gen 3-5 berries with same effect
        switch(_generation)
        {
            case 2:
                if(id >= Items::CHERI_BERRY and id <= Items::ROWAP_BERRY) _gen345_to_gen2();
                break;
            case 3:
            case 4:
            case 5:
                if(id >= Items::BERRY and id <= Items::MYSTERYBERRY) _gen2_to_gen345();
                
            default:
                _item_id = Items::INVALID;
        }
        
        _category_id = database::get_item_category(_item_id);
        _game_index = database::get_item_index(_item_id, _game_id);
        _description = database::get_item_description(_item_id, _game_id);
        _item_name = database::get_item_name(_item_id);
    }
    
    void item_berryimpl::_gen2_to_gen345()
    {
        switch(_item_id)
        {
            case Items::BERRY:
                _item_id = Items::ORAN_BERRY;
                break;
                
            case Items::GOLD_BERRY:
                _item_id = Items::SITRUS_BERRY;
                break;
                
            case Items::PRZCUREBERRY:
                _item_id = Items::CHERI_BERRY;
                break;
                
            case Items::PSNCUREBERRY:
                _item_id = Items::PECHA_BERRY;
                break;
                
            case Items::BITTER_BERRY:
                _item_id = Items::PERSIM_BERRY;
                break;
                
            case Items::BURNT_BERRY:
                _item_id = Items::ASPEAR_BERRY;
                break;
                
            case Items::ICE_BERRY:
                _item_id = Items::RAWST_BERRY;
                break;
            
            case Items::MINT_BERRY:
                _item_id = Items::CHESTO_BERRY;
                break;
            
            case Items::MIRACLEBERRY:
                _item_id = Items::LUM_BERRY;
                break;
                
            case Items::MYSTERYBERRY:
                _item_id = Items::LEPPA_BERRY;
                break;
            
            default: //This should never happen
                _item_id = Items::INVALID;
                break;
        }
    }
    
    void item_berryimpl::_gen345_to_gen2()
    {
        switch(_item_id)
        {
            case Items::ORAN_BERRY:
                _item_id = Items::BERRY;
                break;
                
            case Items::SITRUS_BERRY:
                _item_id = Items::GOLD_BERRY;
                break;
                
            case Items::CHERI_BERRY:
                _item_id = Items::PRZCUREBERRY;
                break;
            
            case Items::PECHA_BERRY:
                _item_id = Items::PSNCUREBERRY;
                break;
            
            case Items::PERSIM_BERRY:
                _item_id = Items::BITTER_BERRY;
                break;
            
            case Items::ASPEAR_BERRY:
                _item_id = Items::BURNT_BERRY;
                break;
            
            case Items::RAWST_BERRY:
                _item_id = Items::ICE_BERRY;
                break;
            
            case Items::CHESTO_BERRY:
                _item_id = Items::MINT_BERRY;
                break;
            
            case Items::LUM_BERRY:
                _item_id = Items::MIRACLEBERRY;
                break;
                
            case Items::LEPPA_BERRY:
                _item_id = Items::MYSTERYBERRY;
                break;
            
            default: //This should never happen
                _item_id = Items::INVALID;
                break;
        }
    }
} /* namespace pkmn */
