/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ABOUTMESSAGEBOX_HPP
#define INCLUDED_ABOUTMESSAGEBOX_HPP

#include <pkmnsim/config.hpp>

#include <QMessageBox>
#include <QLabel>

namespace pkmnsim
{
    namespace qt4
    {
        class PKMNSIM_API AboutMessageBox: public QMessageBox
        {
            Q_OBJECT

            public:
                AboutMessageBox(QWidget* parent = 0);

            /*signals:

            slots:

            private:*/
        };
    }
}

#endif /* INCLUDED_ABOUTMESSAGEBOX_HPP */
