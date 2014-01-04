/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
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

            item_impl(unsigned int id, unsigned int game);

            unsigned int get_game_id();
            unsigned int get_category_id();
            std::string get_category_name();
            unsigned int get_index();
            unsigned int get_item_id();
            std::string get_description();
            std::string get_name();
            
        protected:
            
            unsigned int _item_id, _game_id, _category_id, _game_index;
            std::string _category_name, _description, _item_name;
            
            static SQLite::Database _db;
    };
}

#endif /* INCLUDED_ITEM_IMPL_HPP */
