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
#include "copy_sptrs.hpp"

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
                _pockets["Items"] = pocket::make(_game_id, "Items", 20);
                break;
                
            case Games::GOLD:
            case Games::SILVER:
            case Games::CRYSTAL:
                _pockets["Items"] = pocket::make(_game_id, "Items", 20);
                _pockets["Balls"] = pocket::make(_game_id, "Balls", 12);
                _pockets["KeyItems"] = pocket::make(_game_id, "KeyItems", 26);
                _pockets["TM/HM"] = pocket::make(_game_id, "TM/HM", 99);
                break;
                
            case Games::RUBY:
            case Games::SAPPHIRE:
                _pockets["Items"] = pocket::make(_game_id, "Items", 20);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 46);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 20);
                break;
            
            case Games::EMERALD:
                _pockets["Items"] = pocket::make(_game_id, "Items", 20);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 46);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                break;
                
            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                _pockets["Items"] = pocket::make(_game_id, "Items", 42);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 13);
                _pockets["TM Case"] = pocket::make(_game_id, "TM Case", 58);
                _pockets["Berry Pouch"] = pocket::make(_game_id, "Berry Pouch", 43);
                break;
            
            case Games::DIAMOND:
            case Games::PEARL:
            case Games::PLATINUM:
                _pockets["Items"] = pocket::make(_game_id, "Items", 165);
                _pockets["Medicine"] = pocket::make(_game_id, "Medicine", 40);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 15);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 100);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 64);
                _pockets["Mail"] = pocket::make(_game_id, "Mail", 12);
                _pockets["Battle Items"] = pocket::make(_game_id, "Battle Items", 30);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 50);
                break;

            case Games::HEART_GOLD:
            case Games::SOUL_SILVER:
                _pockets["Items"] = pocket::make(_game_id, "Items", 165);
                _pockets["Medicine"] = pocket::make(_game_id, "Medicine", 40);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 24);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 100);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 64);
                _pockets["Mail"] = pocket::make(_game_id, "Mail", 12);
                _pockets["Battle Items"] = pocket::make(_game_id, "Battle Items", 30);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 43);
                break;
                
            case Games::BLACK:
            case Games::WHITE:
            case Games::BLACK2:
            case Games::WHITE2:
            case Games::X:
            case Games::Y:
                _pockets["Items"] = pocket::make(_game_id, "Items", 310);
                _pockets["Medicine"] = pocket::make(_game_id, "Medicine", 48);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 109);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 64);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 83);
            
            default:
                break;
        }
    }

    bag_impl::bag_impl(const bag_impl& other) :
        _game_id(other._game_id),
        _generation(other._generation)
    {
        std::vector<std::string> pocket_names = other._pockets.keys();
        std::vector<pocket::sptr> pockets = other._pockets.vals();

        for(size_t i = 0; i < pocket_names.size(); i++)
        {
            _pockets[pocket_names[i]] = copy_pocket(pockets[i]);
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
    std::vector<std::vector<std::string> > bag_impl::_category_pockets =
        boost::assign::list_of<std::vector<std::string> >
        (
            boost::assign::list_of //Never used
                ("Items") //Invalid
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
            boost::assign::list_of //RB
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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
                ("Items") //Invalid
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

    std::string bag_impl::get_game() const {return database::get_game_name(_game_id);}

    unsigned int bag_impl::get_generation() const {return _generation;}

    void bag_impl::add_item(pokemon_text item_name, unsigned int amount)
    {
        add_item(database::get_item_id(item_name), amount);
    }

    void bag_impl::add_item(unsigned int item_id, unsigned int amount)
    {
        _pockets[_get_pocket_name(item_id)]->add_item(item_id, amount);
    }

    void bag_impl::add_item(item::sptr item_sptr, unsigned int amount)
    {
        add_item(item_sptr->get_item_id(), amount);
    }

    void bag_impl::remove_item(pokemon_text item_name, unsigned int amount)
    {
        remove_item(database::get_item_id(item_name), amount);
    }

    void bag_impl::remove_item(unsigned int item_id, unsigned int amount)
    {
        _pockets[_get_pocket_name(item_id)]->remove_item(item_id, amount);
    }

    void bag_impl::remove_item(item::sptr item_sptr, unsigned int amount)
    {
        remove_item(item_sptr->get_item_id(), amount);
    }

    unsigned int bag_impl::get_item_amount(pokemon_text item_name) const
    {
        return get_item_amount(database::get_item_id(item_name));
    }

    unsigned int bag_impl::get_item_amount(unsigned int item_id) const
    {
        return _pockets[_get_pocket_name(item_id)]->get_item_amount(item_id);
    }

    unsigned int bag_impl::get_item_amount(item::sptr item_sptr) const
    {
        return get_item_amount(item_sptr->get_item_id());
    }

    pocket::sptr bag_impl::get_pocket(std::string name) const {return _pockets[name];}

    pkmn::dict<std::string, pocket::sptr> bag_impl::get_pockets() const {return _pockets;}

    unsigned int bag_impl::get_game_id() const {return _game_id;}

    //Determine correct pocket for given item
    std::string bag_impl::_get_pocket_name(unsigned int item_id) const
    {
        unsigned int version_group = database::get_version_group(_game_id);

        std::string query_string(str(boost::format("SELECT pocket_id FROM item_categories WHERE id=%d")
                                     % database::get_item_category(item_id)));
        SQLite::Statement pocket_id_query(_db, query_string.c_str());
        unsigned int pocket_id = (pocket_id_query.executeStep()) ? int(pocket_id_query.getColumn(0)) : 0;

        return _category_pockets[version_group][pocket_id];
    }

    void bag_impl::_check() const
    {
        FOREACH(std::string& key, _pockets.keys().begin(), _pockets.keys().end())
        {
            if(_game_id != _pockets[key]->get_game_id())
            {
                throw std::runtime_error(str(boost::format("Pocket \"%s\" is invalid!")
                                             % key));
            }
        }
    }
} /* namespace pkmn */
