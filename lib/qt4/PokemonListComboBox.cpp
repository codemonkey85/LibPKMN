/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmnsim/qt4/PokemonListComboBox.hpp>
#include <pkmnsim/lists.hpp>

using namespace std;

namespace pkmnsim
{
    namespace qt4
    {
        PokemonListComboBox::PokemonListComboBox(QWidget* parent, int game): QComboBox(parent)
        {
            vector<string> pokemon_vec;
            get_pokemon_list(pokemon_vec, game);

            for(unsigned int i = 0; i < pokemon_vec.size(); i++)
            {
                addItem(tr(pokemon_vec[i].c_str()), QVariant(i));
            }
        }
    }
}