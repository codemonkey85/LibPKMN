/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BAG_HPP
#define INCLUDED_PKMN_BAG_HPP

#include <string>

#include <pkmn/bag.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmn
{
    class bag_impl: public bag
    {
        public:

            bag_impl();
            ~bag_impl() {};

            std::string get_game();
            unsigned int get_generation();

            void add_item(pokemon_text item_name, unsigned int amount);
            void add_item(unsigned int item_id, unsigned int amount);
            void add_item(item::sptr item_sptr, unsigned int amount);
            void remove_item(pokemon_text item_name, unsigned int amount);
            void remove_item(unsigned int item_id, unsigned int amount);
            void remove_item(item::sptr item_sptr, unsigned int amount);
            unsigned int get_item_amount(pokemon_text item_name);
            unsigned int get_item_amount(unsigned int item_id);
            unsigned int get_item_amount(item::sptr item_sptr);
            
            pocket::sptr get_pocket(std::string name);
            pkmn::dict<std::string, pocket::sptr> get_pockets();

            unsigned int get_game_id();

        private:

            static SQLite::Database _db;
            static std::vector<std::vector<std::string> > category_pockets;

            unsigned int _game_id, _generation;
            pkmn::dict<std::string, pocket::sptr> _pockets; 
    };
}

#endif /* INCLUDED_PKMN_BAG_HPP */
