/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmnsim/lists.hpp>
#include <pkmnsim/qt4/TypesComboBox.hpp>

using namespace std;

namespace pkmnsim
{
    namespace qt4
    {
        TypesComboBox::TypesComboBox(QWidget* parent, int gen): QComboBox(parent)
        {
            vector<string> types_vec;
            get_type_list(types_vec, gen);

            for(int i = 0; i < types_vec.size(); i++)
            {
                addItem(tr(types_vec[i].c_str()), QVariant(i));
            }
        }
    } /* namespace qt4 */
} /* namespace pkmnsim */