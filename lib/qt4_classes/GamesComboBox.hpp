/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAMESCOMBOBOX_HPP
#define INCLUDED_GAMESCOMBOBOX_HPP

#include <pkmnsim/config.hpp>

#include <QComboBox>

namespace pkmnsim
{
    class PKMNSIM_API GamesComboBox: public QComboBox
    {
        Q_OBJECT

        public:
            GamesComboBox(QWidget* parent = 0, int gen = 5);

        /*signals:

        slots:

        private:*/
    };
}

#endif /* INCLUDED_GAMESCOMBOBOX_HPP */
