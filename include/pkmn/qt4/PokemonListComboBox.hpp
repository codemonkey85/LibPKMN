/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMONLISTCOMBOBOX_HPP
#define INCLUDED_POKEMONLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>
#include <pkmn/enums.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        class PKMN_API PokemonListComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                PokemonListComboBox(unsigned int version_id, QWidget* parent = NULL);
                PokemonListComboBox(std::string &version_name, QWidget* parent = NULL);

            /*signals:

            slots:

            private:*/
        };
    }
}

#endif /* INCLUDED_POKEMONLISTCOMBOBOX_HPP */
