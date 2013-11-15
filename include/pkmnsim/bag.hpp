/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_BAG_HPP
#define INCLUDED_PKMNSIM_BAG_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>

#include <pkmnsim/config.hpp>
#include <pkmnsim/item.hpp>
#include <pkmnsim/pocket.hpp>
#include <pkmnsim/types/dict.hpp>

namespace pkmnsim
{
    class PKMNSIM_API bag
    {
        public:

            typedef std::shared_ptr<bag> sptr;
            static sptr make(unsigned int game);

            bag() {};
            virtual ~bag() {};
            
            virtual unsigned int get_game_id() = 0;
            virtual const pocket::sptr get_pocket(std::string name) = 0;
            virtual void get_pocket_list(std::vector<std::string> &pocket_vec) = 0;
            
        protected:
            
            unsigned int game_id;
            
            dict<std::string, pocket::sptr> contents;
            
    };
}

#endif /* INCLUDED_PKMNSIM_BAG_HPP */
