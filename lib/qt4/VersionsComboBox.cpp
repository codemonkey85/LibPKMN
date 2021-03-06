/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/qt4/VersionsComboBox.hpp>
#include <pkmn/lists.hpp>

namespace pkmn
{
    namespace qt4
    {
        VersionsComboBox::VersionsComboBox(unsigned int gen, QWidget* parent): QComboBox(parent)
        {
            std::vector<std::string> games_vec;
            get_game_group_list(games_vec);

            for(unsigned int i = 0; i < games_vec.size(); i++)
            {
                addItem(tr(games_vec[i].c_str()), QVariant(i));
            }
        }
    } /* namespace qt4 */
} /* namespace pkmn */
