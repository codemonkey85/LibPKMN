/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/lists.hpp>
#include <pkmn/qt4/TypesComboBox.hpp>

namespace pkmn
{
    namespace qt4
    {
        TypesComboBox::TypesComboBox(unsigned int gen, QWidget* parent): QComboBox(parent)
        {
            std::vector<std::string> types_vec;
            get_type_list(types_vec, gen);

            for(int i = 0; i < types_vec.size(); i++)
            {
                addItem(tr(types_vec[i].c_str()), QVariant(i));
            }
        }
    } /* namespace qt4 */
} /* namespace pkmn */
