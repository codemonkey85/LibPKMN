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

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    namespace database
    {
        /*
         * Return a list of tables in the PKMNsim database.
         *
         * Parameters:
         *  - vec: a reference to a vector in which to return list of tables
         */
        void PKMNSIM_API get_table_list(std::vector<std::string> &vec);

        /*
         * Return a list of columns in a given table from the PKMNsim database.
         *
         * Parameters:
         *  - vec: a reference to a vector in which to return list of columns
         *  - table_name: name of table to search
         */
        void PKMNSIM_API get_column_list(std::vector<std::string> &vec, std::string table_name);
    }
}

#endif /* INCLUDED_PKMNSIM_DATABASE_LISTS_HPP */
