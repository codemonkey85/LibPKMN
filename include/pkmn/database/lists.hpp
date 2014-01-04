/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_DATABASE_LISTS_HPP
#define INCLUDED_PKMNSIM_DATABASE_LISTS_HPP

#include <iostream>
#include <string>
#include <vector>

#include <pkmn/config.hpp>

namespace pkmn
{
    namespace database
    {
        void PKMNSIM_API get_table_list(std::vector<std::string> &vec);

        void PKMNSIM_API get_column_list(std::vector<std::string> &vec, std::string table_name);
    }
}

#endif /* INCLUDED_PKMNSIM_DATABASE_LISTS_HPP */
