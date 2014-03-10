/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/dict.hpp>

#include "bag_impl.hpp"

namespace pkmn
{
    bag::sptr bag::make(unsigned int game)
    {
        return sptr(new bag_impl(game));
    }

    bag::sptr bag::make(std::string game)
    {
        //FRLG and HGSS are stored without spaces
        if(game != "Black 2" and game != "White 2") boost::erase_all(game, " ");
        return make(database::get_game_id(game));
    }

    bag_impl::bag_impl(unsigned int game): bag()
    {
        _game_id = game;
        _generation = database::get_generation(_game_id);
        _pockets = pkmn::dict<std::string, pocket::sptr>();
    
        switch(game)
        {
            case Games::RED:
            case Games::BLUE:
            case Games::YELLOW:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                break;
                
            case Games::GOLD:
            case Games::SILVER:
            case Games::CRYSTAL:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                _contents["Balls"] = pocket::make(_game_id, "Balls", 12);
                _contents["KeyItems"] = pocket::make(_game_id, "KeyItems", 26);
                _contents["TM/HM"] = pocket::make(_game_id, "TM/HM", 99);
                break;
                
            case Games::RUBY:
            case Games::SAPPHIRE:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 46);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 20);
                break;
            
            case Games::EMERALD:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 46);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                break;
                
            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                _contents["Items"] = pocket::make(_game_id, "Items", 42);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 13);
                _contents["TM Case"] = pocket::make(_game_id, "TM Case", 58);
                _contents["Berry Pouch"] = pocket::make(_game_id, "Berry Pouch", 43);
                break;
            
            case Games::DIAMOND:
            case Games::PEARL:
            case Games::PLATINUM:
                _contents["Items"] = pocket::make(_game_id, "Items", 165);
                _contents["Medicine"] = pocket::make(_game_id, "Medicine", 40);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 15);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 100);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 64);
                _contents["Mail"] = pocket::make(_game_id, "Mail", 12);
                _contents["Battle Items"] = pocket::make(_game_id, "Battle Items", 30);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 50);
                break;

            case Games::HEART_GOLD:
            case Games::SOUL_SILVER:
                _contents["Items"] = pocket::make(_game_id, "Items", 165);
                _contents["Medicine"] = pocket::make(_game_id, "Medicine", 40);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 24);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 100);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 64);
                _contents["Mail"] = pocket::make(_game_id, "Mail", 12);
                _contents["Battle Items"] = pocket::make(_game_id, "Battle Items", 30);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 43);
                break;
                
            case Games::BLACK:
            case Games::WHITE:
            case Games::BLACK2:
            case Games::WHITE2:
            case Games::X:
            case Games::Y:
                _contents["Items"] = pocket::make(_game_id, "Items", 310);
                _contents["Medicine"] = pocket::make(_game_id, "Medicine", 48);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 109);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 64);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 83);
            
            default:
                break;
        }
    }

    SQLite::Database bag_impl::_db(get_database_path().c_str());

    /*
     * The underlying SQLite database can return item categories, which correspond to bag
     * pockets, but these values are only guaranteed to be accurate for Generation VI.
     * As ugly as this 2D vector is, it's the best way to determine the correct pocket for
     * an item in earlier version groups.
     *
     * The outer vector corresponds to a game's version group, and the inner vector
     * corresponds to the Generation VI item pocket ID.
     */
    std::vector<std::vector<std::string> > bag_impl::category_pockets =
        boost::assign::list_of
        (
            boost::assign::list_of //Never used
                ("Items") //Misc
                ("Items") //Medicine
                ("Items") //Poke Balls
                ("Items") //Machines
                ("Items") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Items") //Key
        }
        boost::assign::list_of
        (
            boost::assign::list_of //RB
                ("Items") //Misc
                ("Items") //Medicine
                ("Items") //Poke Balls
                ("Items") //Machines
                ("Items") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Items") //Key
        )
        (
            boost::assign::list_of //Yellow
                ("Items") //Misc
                ("Items") //Medicine
                ("Items") //Poke Balls
                ("Items") //Machines
                ("Items") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Items") //Key
        )
        (
            boost::assign::list_of //GS
                ("Items") //Misc
                ("Items") //Medicine
                ("Balls") //Poke Balls
                ("TM/HM") //Machines
                ("Items") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("KeyItems") //Key
        )
        (
            boost::assign::list_of //Crystal
                ("Items") //Misc
                ("Items") //Medicine
                ("Balls") //Poke Balls
                ("TM/HM") //Machines
                ("Items") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("KeyItems") //Key
        )
        (
            boost::assign::list_of //RS
                ("Items") //Misc
                ("Items") //Medicine
                ("Poke Balls") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //Emerald
                ("Items") //Misc
                ("Items") //Medicine
                ("Poke Balls") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //FRLG
                ("Items") //Misc
                ("Items") //Medicine
                ("Poke Balls") //Poke Balls
                ("TM Case") //Machines
                ("Berry Pouch") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //DP
                ("Items") //Misc
                ("Medicine") //Medicine
                ("Poke Balls") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Mail") //Mail
                ("Battle Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //Platinum
                ("Items") //Misc
                ("Medicine") //Medicine
                ("Poke Balls") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Mail") //Mail
                ("Battle Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //HGSS
                ("Items") //Misc
                ("Medicine") //Medicine
                ("Poke Balls") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Mail") //Mail
                ("Battle Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //BW
                ("Items") //Misc
                ("Medicine") //Medicine
                ("Items") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //Colosseum
                ("Items") //Misc
                ("Items") //Medicine
                ("Poke Balls") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //XD
                ("Items") //Misc
                ("Items") //Medicine
                ("Poke Balls") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //B2W2
                ("Items") //Misc
                ("Medicine") //Medicine
                ("Items") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
        (
            boost::assign::list_of //XY
                ("Items") //Misc
                ("Medicine") //Medicine
                ("Items") //Poke Balls
                ("TMs and HMs") //Machines
                ("Berries") //Berries
                ("Items") //Mail
                ("Items") //Battle
                ("Key Items") //Key
        )
    ;

    std::string bag_impl::get_game() {return database::get_game_name(_game_id);}

    unsigned int bag_impl::get_generation() {return _generation;}

    unsigned int bag_impl::get_game_id() {return _game_id;}

    void bag_impl::add_item(pokemon_text item_name)
    {
        add_item(database::get_item_id(item_name));
    }

    void bag_impl::add_item(unsigned int item_id)
    {
        //Determine correct pocket for given item
        unsigned int version_group = database::get_version_group(_game_id);

        std::string query_string(str(boost::format("SELECT pocket_id FROM item_categories WHERE id=%d")
                                     % database::get_item_category(item_id)));
        SQLite::Statement pocket_id_query(_db, query_string.c_str());
        unsigned int pocket_id = (pocket_id_query.executeStep()) ? int(pocket_id_query.getColumn(0)) : 0;

        _pockets[category_pockets[version_group][pocket_id]].add_item(item_id);
    }

    void bag_impl::add_item(item::sptr item_sptr)
    {
        add_item(item_sptr->get_item_id());
    }
    
    pocket::sptr bag_impl::get_pocket(std::string name) {return _contents[name];}
    
    void bag_impl::get_pocket_list(std::vector<std::string> &pocket_vec) {pocket_vec = _contents.keys();}

    void bag_impl::_check() const
    {
        FOREACH(std::string& key, _contents.keys().begin(), _contents.keys().end())
        {
            if(_game_id != _contents[key]->get_game_id())
            {
                throw std::runtime_error(str(boost::format("Pocket \"%s\" is invalid!")
                                             % key));
            }
        }
    }
} /* namespace pkmn */
