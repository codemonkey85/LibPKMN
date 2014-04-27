/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BAG_IMPL_HPP
#define INCLUDED_BAG_IMPL_HPP

#include <string>

#include <pkmn/bag.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmn
{
    class bag_impl: public bag
    {
        public:

            bag_impl(unsigned int game);
            bag_impl(const bag_impl& other);
            bag_impl& operator=(const bag_impl& other);
            ~bag_impl() {};

            std::string get_game() const;
            unsigned int get_generation() const;

            void add_item(pokemon_text item_name, unsigned int amount);
            void add_item(unsigned int item_id, unsigned int amount);
            void add_item(item::sptr item_sptr, unsigned int amount);
            void remove_item(pokemon_text item_name, unsigned int amount);
            void remove_item(unsigned int item_id, unsigned int amount);
            void remove_item(item::sptr item_sptr, unsigned int amount);
            unsigned int get_item_amount(pokemon_text item_name) const;
            unsigned int get_item_amount(unsigned int item_id) const;
            unsigned int get_item_amount(item::sptr item_sptr) const;
            
            pocket::sptr get_pocket(std::string name) const;
            pkmn::dict<std::string, pocket::sptr> get_pockets() const;

            unsigned int get_game_id() const;

        private:

            static SQLite::Database _db;
            static std::vector<std::vector<std::string> > _category_pockets;

            unsigned int _game_id, _generation;
            pkmn::dict<std::string, pocket::sptr> _pockets; 

            std::string _get_pocket_name(unsigned int item_id) const;
            void _check() const;
    };
}

#endif /* INCLUDED_BAG_IMPL_HPP */
