/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_PATHS_HPP
#define INCLUDED_PKMNSIM_PATHS_HPP

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*
     * Returns a Pokemon's name in a format compatible with querying the database.
     */
    std::string PKMNSIM_API to_database_format(std::string identifier);

    /*
     * Returns the path to the PKMNsim SQLite database.
     */
    std::string PKMNSIM_API get_database_path(void);

    /*
     * Returns the path to the directory where all images are stored.
     */
    std::string PKMNSIM_API get_images_dir(void);
}

#endif /* INCLUDED_PKMNSIM_PATHS_HPP */
