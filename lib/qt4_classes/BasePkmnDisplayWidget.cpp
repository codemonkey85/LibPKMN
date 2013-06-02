/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/BasePkmnDisplayWidget.hpp>
#include "../internal.hpp"

#include <boost/filesystem.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    BasePkmnDisplayWidget::BasePkmnDisplayWidget(QWidget* parent, base_pkmn::sptr pkmn): QWidget(parent)
    {
        //TODO: Check for valid base_pkmn in case default is used
        b_pkmn = pkmn;
        
        //Declare layouts and labels
        QHBoxLayout* mainLayout = new QHBoxLayout(this);
        QLabel* leftLabel = new QLabel(this);

        //Define QString for rightLabel
        QString rightString = QString("%1 (#%2)").arg(
            QString::fromStdString(b_pkmn->get_display_name()),
            QString::number(b_pkmn->get_nat_pokedex_num())
        );

        //Eventually replace with get_sprite_path() for Unown and other anomalies
        string icon_name = to_string(b_pkmn->get_nat_pokedex_num()) + ".png";
        fs::path pkmn_icon_path = fs::path(get_images_dir().c_str()) / "icons" / icon_name.c_str();

        QImage spriteImage;
        spriteImage.load(QString(pkmn_icon_path.string().c_str()));

        leftLabel->setPixmap(QPixmap::fromImage(spriteImage));
        QLabel* rightLabel = new QLabel(rightString);

        mainLayout->addWidget(leftLabel);        
        mainLayout->addWidget(rightLabel);

        setLayout(mainLayout);

        setMaximumSize(200,50);
    }
}
