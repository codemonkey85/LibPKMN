/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_MOVE_MAINIMPL_HPP
#define INCLUDED_BASE_MOVE_MAINIMPL_HPP

#include <boost/format.hpp>
#include <pkmnsim/base_move.hpp>
#include "sqlitecpp/SQLiteCPP.h"
#include <string>

using namespace std;

namespace pkmnsim
{
    class PKMNSIM_API base_move_mainimpl: public base_move
    {
        public:
            base_move_mainimpl(std::string identifier, SQLite::Database *db, int gen);
    };
}

#endif /* INCLUDED_BASE_MOVE_MAINIMPL_HPP */
