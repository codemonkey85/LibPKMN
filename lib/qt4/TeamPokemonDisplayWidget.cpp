/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/qt4/TeamPokemonDisplayWidget.hpp>

#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace pkmn
{
    namespace qt4
    {
        TeamPokemonDisplayWidget::TeamPokemonDisplayWidget(QWidget* parent, team_pokemon::sptr pkmn): QWidget(parent)
        {
            //TODO: Check for valid team_pokemon in case default is used
            t_pkmn = pkmn;
            int game = t_pkmn->get_game_id();
            
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
            switch(t_pkmn->get_gender())
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
            if(database::get_generation(game) == 1) pokemonQString = QString("%1 (%2)").arg(
                QString::fromStdString(t_pkmn->get_species_name()),
                QString::fromStdString(t_pkmn->get_nickname().std_string())
            );
            else pokemonQString = QString("%1 (%2 - %3)").arg(
                QString::fromStdString(t_pkmn->get_species_name()),
                QString::fromStdString(t_pkmn->get_nickname().std_string()),
                gender
            );
            QLabel* pokemonLabel = new QLabel(pokemonQString);
            
            QString itemQString; //items not implemented, defaulting to None
            QLabel* itemLabel;
            if(database::get_generation(game) >= 2)
            {
                itemQString = QString("Item Held: None");
                itemLabel = new QLabel(itemQString);
            }
            
            QString abilityNatureQString;
            QLabel* abilityNatureLabel;
            if(database::get_generation(game) >= 3)
            {
                abilityNatureQString = QString("Ability: %1, Nature: %2").arg(
                    QString::fromStdString(database::get_ability_name(t_pkmn->get_ability())),
                    QString::fromStdString(database::get_nature_name(t_pkmn->get_nature()))
                );
                abilityNatureLabel = new QLabel(abilityNatureQString);
            }
            
            moveset_t moves = t_pkmn->get_moves();
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
            if(database::get_generation(game) >= 2)
            {
                itemLayout->addWidget(itemLabel);
                mainLayout->addLayout(itemLayout);
            }
            if(database::get_generation(game) >= 3)
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
    } /* namespace qt4 */
} /* namespace pkmn */