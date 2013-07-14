/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_DATABASE_QUERIES_HPP
#define INCLUDED_DATABASE_QUERIES_HPP

#include <string>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    namespace database
    {
        /*
         * Returns given string in a format to be used with database
         */
        std::string to_database_format(std::string identifier);
    }
}

#endif /* INCLUDED_DATABASE_QUERIES_HPP */
