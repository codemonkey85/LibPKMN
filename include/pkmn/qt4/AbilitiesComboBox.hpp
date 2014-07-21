/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ABILITIESCOMBOBOX_HPP
#define INCLUDED_ABILITIESCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        class PKMN_API AbilitiesComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                AbilitiesComboBox(unsigned int gen, QWidget* parent = NULL);

            /*signals:

            slots:

            private:*/
        };
    }
}

#endif /* INCLUDED_ABILITIESCOMBOBOX_HPP */
