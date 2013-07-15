/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/paths.hpp>
#include <pkmnsim/SpecPkmnDisplayWidget.hpp>

#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace pkmnsim
{
    SpecPkmnDisplayWidget::SpecPkmnDisplayWidget(QWidget* parent, spec_pkmn::sptr pkmn): QWidget(parent)
    {
        //TODO: Check for valid spec_pkmn in case default is used
        s_pkmn = pkmn;
        QString images_dir = QString("%1").arg(
            QString::fromStdString(get_images_dir())
        );
        int gen = s_pkmn->get_generation();
        
        //Declare layouts and labels
        QVBoxLayout* mainLayout = new QVBoxLayout();
        QHBoxLayout* pokemonLayout = new QHBoxLayout();
        
        QHBoxLayout* itemLayout = new QHBoxLayout();
        
        QHBoxLayout* abilityNatureLayout = new QHBoxLayout();
        
        QHBoxLayout* attackOneLayout = new QHBoxLayout();
        QHBoxLayout* attackTwoLayout = new QHBoxLayout();
        QHBoxLayout* attackThreeLayout = new QHBoxLayout();
        QHBoxLayout* attackFourLayout = new QHBoxLayout();

        //Get gender character
        QChar gender;
        switch(s_pkmn->get_gender())
        {
            case Genders::MALE:
                gender = QChar('M');
                break;

            case Genders::FEMALE:
                gender = QChar('F');
                break;

            default:
                gender = QChar('N');
                break;
        }
        
        //Fill labels
        QString pokemonQString;
        if(gen == 1) pokemonQString = QString("%1 (%2)").arg(
            QString::fromStdString(s_pkmn->get_species_name()),
            QString::fromStdString(s_pkmn->get_nickname())
        );
        else pokemonQString = QString("%1 (%2 - %3)").arg(
            QString::fromStdString(s_pkmn->get_species_name()),
            QString::fromStdString(s_pkmn->get_nickname()),
            gender
        );
        QLabel* pokemonLabel = new QLabel(pokemonQString);
        
        QString itemQString; //items not implemented, defaulting to None
        QLabel* itemLabel;
        if(gen >= 2)
        {
            itemQString = QString("Item Held: None");
            itemLabel = new QLabel(itemQString);
        }
        
        QString abilityNatureQString;
        QLabel* abilityNatureLabel;
        if(gen >= 3)
        {
            abilityNatureQString = QString("Ability: %1, Nature: %2").arg(
                QString::fromStdString(s_pkmn->get_ability()),
                QString::fromStdString(s_pkmn->get_nature()->get_name())
            );
            abilityNatureLabel = new QLabel(abilityNatureQString);
        }
        
        vla<base_move::sptr> moves = s_pkmn->get_moves();
        QString attackOneQString = QString("%1").arg(
            QString::fromStdString(moves[0]->get_name())
        );
        QString attackTwoQString = QString("%1").arg(
            QString::fromStdString(moves[1]->get_name())
        );
        QString attackThreeQString = QString("%1").arg(
            QString::fromStdString(moves[2]->get_name())
        );
        QString attackFourQString = QString("%1").arg(
            QString::fromStdString(moves[3]->get_name())
        );
        QLabel* attackOneLabel = new QLabel(attackOneQString);
        QLabel* attackTwoLabel = new QLabel(attackTwoQString);
        QLabel* attackThreeLabel = new QLabel(attackThreeQString);
        QLabel* attackFourLabel = new QLabel(attackFourQString);

        //Populate layouts
        pokemonLayout->addWidget(pokemonLabel);
        attackOneLayout->addWidget(attackOneLabel);
        attackTwoLayout->addWidget(attackTwoLabel);
        attackThreeLayout->addWidget(attackThreeLabel);
        attackFourLayout->addWidget(attackFourLabel);
        
        mainLayout->addLayout(pokemonLayout);
        if(gen >= 2)
        {
            itemLayout->addWidget(itemLabel);
            mainLayout->addLayout(itemLayout);
        }
        if(gen >= 3)
        {
            abilityNatureLayout->addWidget(abilityNatureLabel);
            mainLayout->addLayout(abilityNatureLayout);
        }
        mainLayout->addLayout(attackOneLayout);
        mainLayout->addLayout(attackTwoLayout);
        mainLayout->addLayout(attackThreeLayout);
        mainLayout->addLayout(attackFourLayout);
        
        setLayout(mainLayout);
    }
}
