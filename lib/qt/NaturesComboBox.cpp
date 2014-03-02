/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/lists.hpp>
#include <pkmn/qt/NaturesComboBox.hpp>

using namespace std;

namespace pkmn
{
    namespace qt
    {
        NaturesComboBox::NaturesComboBox(QWidget* parent): QComboBox(parent)
        {
            vector<string> natures_vec;
            get_nature_list(natures_vec);

            for(int i = 0; i < natures_vec.size(); i++)
            {
                addItem(tr(natures_vec[i].c_str()), QVariant(i));
            }
        }
    } /* namespace qt */
} /* namespace pkmn */