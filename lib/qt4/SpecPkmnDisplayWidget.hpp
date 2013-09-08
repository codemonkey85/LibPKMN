/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_SPECPKMNDISPLAYWIDGET_HPP
#define INCLUDED_SPECPKMNDISPLAYWIDGET_HPP

#include <pkmnsim/config.hpp>
#include <pkmnsim/spec_pokemon.hpp>
#include <pkmnsim/vla.hpp>

#include <QChar>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace pkmnsim
{
    namespace qt4
    {
        class PKMNSIM_API SpecPkmnDisplayWidget: public QWidget
        {
            Q_OBJECT

            public:
                SpecPkmnDisplayWidget(QWidget* parent = 0, spec_pokemon::sptr pkmn = *(new spec_pokemon::sptr));
               
            /*signals:

            slots:*/

            private:
                spec_pokemon::sptr s_pkmn;
        };
    }
}

#endif /* INCLUDED_SPECPKMNDISPLAYWIDGET_HPP */
