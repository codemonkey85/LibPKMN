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
    std::string PKMNSIM_API get_appdata_path();
    std::string PKMNSIM_API get_database_path();
    std::string PKMNSIM_API get_images_dir();
    std::string PKMNSIM_API get_tmp_dir();
}

#endif /* INCLUDED_PKMNSIM_PATHS_HPP */
