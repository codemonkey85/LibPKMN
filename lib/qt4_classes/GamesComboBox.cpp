/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmnsim/GamesComboBox.hpp>
#include <pkmnsim/lists.hpp>

using namespace std;

namespace pkmnsim
{
    GamesComboBox::GamesComboBox(QWidget* parent, int gen): QComboBox(parent)
    {
        vector<string> games_vec;
        get_game_group_list(games_vec);

        for(int i = 0; i < games_vec.size(); i++)
        {
            addItem(tr(games_vec[i].c_str()), QVariant(i));
        }
    }
}
