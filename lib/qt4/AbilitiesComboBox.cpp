/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmnsim/qt4/AbilitiesComboBox.hpp>
#include <pkmnsim/lists.hpp>

using namespace std;

namespace pkmnsim
{
    namespace qt4
    {
        AbilitiesComboBox::AbilitiesComboBox(QWidget* parent, int gen): QComboBox(parent)
        {
            vector<string> abilities_vec;
            get_ability_list(abilities_vec, gen);

            for(unsigned int i = 0; i < abilities_vec.size(); i++)
            {
                addItem(tr(abilities_vec[i].c_str()), QVariant(i));
            }
        }
    }
}
