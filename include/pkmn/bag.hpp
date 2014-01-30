/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BAG_HPP
#define INCLUDED_PKMN_BAG_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/item.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API bag
    {
        public:

            typedef pkmn::shared_ptr<bag> sptr;
            static sptr make(unsigned int game);
            static sptr make(std::string game);

            bag() {};
            virtual ~bag() {};
            
            virtual unsigned int get_game_id() = 0;
            virtual pocket::sptr get_pocket(std::string name) = 0;
            virtual void get_pocket_list(std::vector<std::string> &pocket_vec) = 0;
    };
}

#endif /* INCLUDED_PKMN_BAG_HPP */
