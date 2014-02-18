/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_PATHS_HPP
#define INCLUDED_PKMN_PATHS_HPP

#include <string>
#include <vector>

#include <pkmn/config.hpp>

namespace pkmn
{
    /*!
     * Return path to user's local application data directory. In UNIX-like systems,
     * this is the user's home directory, while in Windows systems, this is the AppData
     * directory in the user's home directory. If, somehow, neither of these are true,
     * the function returns "/tmp".
     *
     * \return user's application data directory
     */
    std::string PKMN_API get_appdata_path();

    /*!
     * Return the path to LibPKMN's underlying SQLite database. Under default conditions,
     * this path is:
     * <pre>
     * (INSTALL PREFIX)/share/libpkmn/libpkmn.db
     * </pre>
     *
     * If the function cannot find the database, it returns the string ":memory:", which creates
     * a new SQLite database in-memory.
     *
     * \return path to underlying SQLite database
     */
    std::string PKMN_API get_database_path();

    /*!
     * Return the path to the top-level directory of LibPKMN's images. Under default conditions,
     * this path is:
     * <pre>
     * (INSTALL PREFIX)/share/libpkmn/images
     * </pre>
     *
     * \return path to LibPKMN's images directory
     */
    std::string PKMN_API get_images_dir();

    /*!
     * Return the system's temporary directory.
     *
     * \return system's temp directory
     */
    std::string PKMN_API get_tmp_dir();
}

#endif /* INCLUDED_PKMN_PATHS_HPP */
