/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ITEM_IMPL_HPP
#define INCLUDED_ITEM_IMPL_HPP

#include <pkmn/item.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmn
{
    class item_impl: public item
    {
        public:

            item_impl();
            item_impl(unsigned int id, unsigned int game);

            //Game-specific Info
            std::string get_game() const;
            unsigned int get_generation() const;

            std::string get_name() const;
            std::string get_description() const;
            std::string get_category() const;
            unsigned int get_game_index() const;

            //Database Info
            unsigned int get_item_id() const;
            unsigned int get_category_id() const;
            unsigned int get_game_id() const;

        protected:

            unsigned int _item_id, _game_id, _category_id, _game_index, _generation;
            std::string _category_name, _description, _item_name;

            static SQLite::Database _db;
    };
}

#endif /* INCLUDED_ITEM_IMPL_HPP */
