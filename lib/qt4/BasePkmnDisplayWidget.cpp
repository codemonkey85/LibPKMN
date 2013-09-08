/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/qt4/BasePkmnDisplayWidget.hpp>

#include <boost/filesystem.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    namespace qt4
    {
        BasePkmnDisplayWidget::BasePkmnDisplayWidget(QWidget* parent, base_pokemon::sptr pkmn): QWidget(parent)
        {
            //TODO: Check for valid base_pokemon in case default is used
            b_pkmn = pkmn;
            
            //Declare layouts and labels
            QHBoxLayout* mainLayout = new QHBoxLayout(this);
            QLabel* leftLabel = new QLabel(this);
            QSizePolicy mainLayoutPolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

            //Define QString for rightLabel
            QString rightString = QString("%1").arg(
                QString::fromStdString(b_pkmn->get_species_name())
            );

            QImage spriteImage;
            spriteImage.load(QString(pkmn->get_icon_path(true).c_str()));

            leftLabel->setPixmap(QPixmap::fromImage(spriteImage));
            leftLabel->setFixedSize(spriteImage.size());
            QLabel* rightLabel = new QLabel(rightString);

            rightLabel->setSizePolicy(mainLayoutPolicy);

            mainLayout->addWidget(leftLabel);        
            mainLayout->addWidget(rightLabel);

            setLayout(mainLayout);
        }
    }
}
