/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_INTERNAL_HPP
#define INCLUDED_INTERNAL_HPP

#include <string>

namespace pkmnsim
{
    void to_database_format(std::string *identifier);
    std::string get_database_path(void);
    std::string get_images_dir(void);
}

#endif /* INCLUDED_INTERNAL_HPP */
