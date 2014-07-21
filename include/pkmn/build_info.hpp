/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BUILD_INFO_HPP
#define INCLUDED_PKMN_BUILD_INFO_HPP

#include <string>

#include <pkmn/config.hpp>

namespace pkmn
{
    std::string PKMN_API get_sqlite3_version();

    std::string PKMN_API get_sqlitecpp_version();

    std::string PKMN_API get_boost_version();

    std::string PKMN_API get_qt4_version();

    std::string PKMN_API get_libpkmn_commit();

    std::string PKMN_API get_rpokesav_commit();

    std::string PKMN_API get_pokelib_nc_commit();

    std::string PKMN_API get_pkmds_commit();
}

#endif /* INCLUDED_PKMN_BUILD_INFO_HPP */
