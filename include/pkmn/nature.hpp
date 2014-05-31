/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATURE_HPP
#define INCLUDED_PKMN_NATURE_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>

namespace pkmn
{
    class PKMN_API nature
    {
        public:

            nature() {};
            nature(unsigned int id);
            nature(std::string name);
            ~nature() {};

            std::string get_name();

            double& operator[](const std::string &key);

            unsigned int get_nature_id();

        private:

            unsigned int _id;
            std::string _name;

            pkmn::dict<std::string, double> _modifiers;

            void _populate_dict();
    };
}

#endif /* INCLUDED_PKMN_NATURE_HPP */
