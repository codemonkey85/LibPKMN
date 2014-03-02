/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMONLISTCOMBOBOX_HPP
#define INCLUDED_POKEMONLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt
    {
        class PKMN_API PokemonListComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                PokemonListComboBox(QWidget* parent = 0, int game = 24);

            /*signals:

            slots:

            private:*/
        };
    }
}

#endif /* INCLUDED_POKEMONLISTCOMBOBOX_HPP */
