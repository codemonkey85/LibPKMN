/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/qt4/AbilitiesComboBox.hpp>
#include <pkmn/lists.hpp>

namespace pkmn
{
    namespace qt4
    {
        AbilitiesComboBox::AbilitiesComboBox(unsigned int gen, QWidget* parent): QComboBox(parent)
        {
            std::vector<std::string> abilities_vec;
            get_ability_list(abilities_vec, gen);

            for(unsigned int i = 0; i < abilities_vec.size(); i++)
            {
                addItem(tr(abilities_vec[i].c_str()), QVariant(i));
            }
        }
    }
} /* namespace pkmn */
