/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ITEM_HPP
#define INCLUDED_PKMN_ITEM_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    /*!
     * This class represents an in-game <a href="http://bulbapedia.bulbagarden.net/wiki/Item">item</a>.
     * The factory function takes in an item name/ID and game name/ID, and returns a class
     * instance from which various information about the item can be queried.
     *
     * To instantiate an item shared_ptr, you can use one of two methods:
     * <pre>
     * pkmn::item::sptr pkmn_item = pkmn::item::make(pkmn::Items::POTION, pkmn::Games::CRYSTAL)
     * pkmn::item::sptr pkmn_item = pkmn::item::make("Potion", "Crystal")
     * </pre>
     */
    class PKMN_API item
    {
        public:

            typedef pkmn::shared_ptr<item> sptr;

            //! Class's factory function (using SQLite ID's)
            /*!
             * This is the class's factory function. It takes in the SQLite database ID for the game
             * and item this class should describe.
             *
             * \param id SQLite database ID of the desired item, as defined in <pkmn/enums.hpp>
             * \param game SQLite database ID of the item's game, as defined in <pkmn/enums.hpp>
             * \return shared pointer to instance of pkmn::item with described parameters
             */
            static sptr make(unsigned int id, unsigned int game);

            //! Class's factory function
            /*!
             * This is the class's factory function. It takes in the name of the game
             * and item this class should describe.
             *
             * \param name item's name
             * \param game item's game
             * \return shared pointer to instance of pkmn::item with described parameters
             */
            static sptr make(std::string name, std::string game);

            //! Class constructor (use factory function instead)
            item() {};
            virtual ~item() {};

            //! Return the name of the item from which this item was created (ex. "Red", "Diamond").
            virtual std::string get_game() const = 0;

            //! Return the generation of the item from which this item was created (1-6).
            virtual unsigned int get_generation() const = 0;

            //! Return the item's name.
            virtual std::string get_name() const = 0;

            //! Return the item's description (varies between games).
            virtual std::string get_description() const = 0;

            //! Return the item's category (ex. "Medicine", "Key Items").
            virtual std::string get_category() const = 0;

            //! Return the items in-game index value (varies between version groups).
            virtual unsigned int get_game_index() const = 0;
            
            //! Return item's SQLite database ID.
            virtual unsigned int get_item_id() const = 0;

            //! Return item category's SQLite database ID.
            virtual unsigned int get_category_id() const = 0;

            //! Return SQLite Database ID of game used to create this item instance.
            virtual unsigned int get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_ITEM_HPP */
