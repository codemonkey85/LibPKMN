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
    class PKMN_API item
    {
        public:

            typedef pkmn::shared_ptr<item> sptr;
            static sptr make(unsigned int id, unsigned int game);
            static sptr make(std::string name, std::string game);

            item() {};
            virtual ~item() {};

            //Game-specific Info
            virtual std::string get_game() const = 0;
            virtual unsigned int get_generation() const = 0;
            
            virtual std::string get_name() const = 0;
            virtual std::string get_description() const = 0;
            virtual std::string get_category() const = 0;
            virtual unsigned int get_game_index() const = 0;
            
            //Database Info
            virtual unsigned int get_item_id() const = 0;
            virtual unsigned int get_category_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_ITEM_HPP */
