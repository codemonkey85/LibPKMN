/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TYPESCOMBOBOX_HPP
#define INCLUDED_TYPESCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        class PKMN_API NaturesComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                NaturesComboBox(QWidget* parent = NULL);

            /*signals:

            slots:

            private:*/
        };
    }
}

#endif /* INCLUDED_TYPESCOMBOBOX_HPP */
