/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/database/queries.hpp>
#include <pkmn/qt4/PokemonListComboBox.hpp>
#include <pkmn/lists.hpp>

namespace pkmn
{
    namespace qt4
    {
        PokemonListComboBox::PokemonListComboBox(unsigned int version_id, QWidget* parent): QComboBox(parent)
        {
            std::vector<std::string> pokemon_vec;
            get_pokemon_list(pokemon_vec, version_id);

            for(unsigned int i = 0; i < pokemon_vec.size(); i++)
            {
                addItem(tr(pokemon_vec[i].c_str()), QVariant(i));
            }
        }

        PokemonListComboBox::PokemonListComboBox(std::string &version_name, QWidget* parent): QComboBox(parent)
        {
            PokemonListComboBox(database::get_version_id(version_name), parent);
        }

    } /* namespace qt4 */
} /* namespace pkmn */
