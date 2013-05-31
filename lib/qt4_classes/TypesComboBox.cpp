/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <pkmnsim/lists.hpp>
#include <pkmnsim/TypesComboBox.hpp>

#include <string>
#include <vector>

using namespace std;

namespace pkmnsim
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
}