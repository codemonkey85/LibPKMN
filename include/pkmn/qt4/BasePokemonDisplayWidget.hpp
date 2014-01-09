/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_QT4_BASEPOKEMONDISPLAYWIDGET_HPP
#define INCLUDED_PKMN_QT4_BASEPOKEMONDISPLAYWIDGET_HPP

#include <pkmn/config.hpp>
#include <pkmn/base_pokemon.hpp>

#include <QChar>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QWidget>

namespace pkmn
{
    namespace qt4
    {
        class PKMN_API BasePokemonDisplayWidget: public QWidget
        {
            Q_OBJECT
            
            public:
                BasePokemonDisplayWidget(QWidget* parent = 0, base_pokemon::sptr pkmn = *(new base_pokemon::sptr));
                
            /*signals:
            
            slots:*/
            
            private:
                base_pokemon::sptr b_pkmn;
        };
    }
}

#endif /* INCLUDED_PKMN_QT4_BASEPOKEMONDISPLAYWIDGET_HPP */
