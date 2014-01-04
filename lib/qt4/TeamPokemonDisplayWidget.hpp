/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_QT4_TEAMPOKEMONDISPLAYWIDGET_HPP
#define INCLUDED_PKMNSIM_QT4_TEAMPOKEMONDISPLAYWIDGET_HPP

#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>

#include <QChar>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace pkmn
{
    namespace qt4
    {
        class PKMNSIM_API TeamPokemonDisplayWidget: public QWidget
        {
            Q_OBJECT

            public:
                TeamPokemonDisplayWidget(QWidget* parent = 0, team_pokemon::sptr pkmn = *(new team_pokemon::sptr));
               
            /*signals:

            slots:*/

            private:
                team_pokemon::sptr t_pkmn;
        };
    }
}

#endif /* INCLUDED_PKMNSIM_QT4_TEAMPOKEMONDISPLAYWIDGET_HPP */
