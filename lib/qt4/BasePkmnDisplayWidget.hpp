/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASEPKMNDISPLAYWIDGET_HPP
#define INCLUDED_BASEPKMNDISPLAYWIDGET_HPP

#include <pkmnsim/config.hpp>
#include <pkmnsim/base_pkmn.hpp>

#include <QChar>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QWidget>

namespace pkmnsim
{
    namespace qt4
    {
        class PKMNSIM_API BasePkmnDisplayWidget: public QWidget
        {
            Q_OBJECT
            
            public:
                BasePkmnDisplayWidget(QWidget* parent = 0, base_pkmn::sptr pkmn = *(new base_pkmn::sptr));
                
            /*signals:
            
            slots:*/
            
            private:
                base_pkmn::sptr b_pkmn;
        };
    }
}

#endif /* INCLUDED_BASEPKMNDISPLAYWIDGET_HPP */
