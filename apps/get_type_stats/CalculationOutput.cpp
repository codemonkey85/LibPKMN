/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "CalculationOutput.hpp"

using namespace pkmnsim;
using namespace std;

CalculationOutput::CalculationOutput(QWidget* parent, int gen): QWidget(parent)
{
    generation = gen;

    QVBoxLayout* mainLayout = new QVBoxLayout();

    QGroupBox* hpGroupBox = new QGroupBox(tr("HP"),this);
    hpGroupBox->setObjectName("hpGroupBox");
    QHBoxLayout* hpLayout = new QHBoxLayout(hpGroupBox);
    QLabel* hpEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
    hpLayout->addWidget(hpEmptyLabel);
    hpLayout->setAlignment(Qt::AlignCenter);
    
    QGroupBox* attackGroupBox = new QGroupBox(tr("Attack"),this);
    attackGroupBox->setObjectName("attackGroupBox");
    QHBoxLayout* attackLayout = new QHBoxLayout(attackGroupBox);
    QLabel* attackEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
    attackLayout->addWidget(attackEmptyLabel);
    attackLayout->setAlignment(Qt::AlignCenter);

    QGroupBox* defenseGroupBox = new QGroupBox(tr("Defense"),this);
    defenseGroupBox->setObjectName("defenseGroupBox");
    QHBoxLayout* defenseLayout = new QHBoxLayout(defenseGroupBox);
    QLabel* defenseEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
    defenseLayout->addWidget(defenseEmptyLabel);
    defenseLayout->setAlignment(Qt::AlignCenter);

    QGroupBox* speedGroupBox = new QGroupBox(tr("Speed"),this);
    speedGroupBox->setObjectName("defenseGroupBox");
    QHBoxLayout* speedLayout = new QHBoxLayout(speedGroupBox);
    QLabel* speedEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
    speedLayout->addWidget(speedEmptyLabel);
    speedLayout->setAlignment(Qt::AlignCenter);

    hpGroupBox->setLayout(hpLayout);
    attackGroupBox->setLayout(attackLayout);
    defenseGroupBox->setLayout(defenseLayout);
    speedGroupBox->setLayout(speedLayout);
    
    mainLayout->addWidget(hpGroupBox);
    mainLayout->addWidget(attackGroupBox);
    mainLayout->addWidget(defenseGroupBox);
    mainLayout->addWidget(speedGroupBox);
    
    if(gen == 1)
    {
        QGroupBox* specialGroupBox = new QGroupBox(tr("Special"),this);
        specialGroupBox->setObjectName("specialGroupBox");
        QHBoxLayout* specialLayout = new QHBoxLayout(specialGroupBox);
        specialGroupBox->setLayout(specialLayout);
        QLabel* specialEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
        specialLayout->addWidget(specialEmptyLabel);
        specialLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(specialGroupBox);
    }
    else
    {
        QGroupBox* specialAttackGroupBox = new QGroupBox(tr("Special Attack"),this);
        specialAttackGroupBox->setObjectName("specialAttackGroupBox");
        QHBoxLayout* specialAttackLayout = new QHBoxLayout(specialAttackGroupBox);
        specialAttackGroupBox->setLayout(specialAttackLayout);
        QLabel* specialAttackEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
        specialAttackLayout->addWidget(specialAttackEmptyLabel);
        specialAttackLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(specialAttackGroupBox);

        QGroupBox* specialDefenseGroupBox = new QGroupBox(tr("Special Defense"),this);
        specialDefenseGroupBox->setObjectName("specialDefenseGroupBox");
        QHBoxLayout* specialDefenseLayout = new QHBoxLayout(specialDefenseGroupBox);
        specialDefenseGroupBox->setLayout(specialDefenseLayout);
        QLabel* specialDefenseEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
        specialDefenseLayout->addWidget(specialDefenseEmptyLabel);
        specialDefenseLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(specialDefenseGroupBox);
    }
    
    setLayout(mainLayout);
}

//Slot to call when results are calculated by OptionsGroupBox
void CalculationOutput::getAndShowResults(vector<vector<stat_st> > highest_stats_vecs,
                                          vector<vector<stat_st> > lowest_stats_vecs,
                                          vector<int> errcodes, string type1, string type2)
{
    vector<stat_st> high_vec = highest_stats_vecs[generation-1];
    vector<stat_st> low_vec = lowest_stats_vecs[generation-1];

    QList<QGroupBox*> groupBoxQList = this->findChildren<QGroupBox*>();
    for(int i = 0; i < groupBoxQList.count(); i++)
    {
        //Delete all widgets in QGroupBox
        QList<QLabel*> labelQList = groupBoxQList.at(i)->findChildren<QLabel*>();
        for(int j = 0; j < labelQList.count(); j++) delete labelQList.at(j);
        QList<BasePkmnDisplayWidget*> basePkmnQList = groupBoxQList.at(i)->findChildren<BasePkmnDisplayWidget*>();
        for(int j = 0; j < basePkmnQList.count(); j++) delete basePkmnQList.at(j);
        QFrame* delVertLine = groupBoxQList.at(i)->findChild<QFrame*>(QString("vertLine"));
        if(delVertLine) delete delVertLine;

        if(errcodes[generation-1] or high_vec[i].pkmn_name == "Missingno." or low_vec[i].pkmn_name == "Missingno.")
        {
            QLabel* noPkmnLabel = new QLabel(QString("No Pokemon of specified type combination in Generation %1").arg(
                                                QString::number(generation)
                                            ));
            groupBoxQList.at(i)->layout()->addWidget(noPkmnLabel);
        }
        else
        {
            //Add appropriate Pokemon
            base_pkmn::sptr highPkmn = base_pkmn::make(high_vec[i].pkmn_name, generation, false);
            base_pkmn::sptr lowPkmn = base_pkmn::make(low_vec[i].pkmn_name, generation, false);

            //Manually set Castform form if necessary
            if(highPkmn->get_display_name() == "Castform") set_castform_type(highPkmn, type1);
            if(lowPkmn->get_display_name() == "Castform") set_castform_type(lowPkmn, type1);

            //Create BasePkmnDisplayWidgets
            BasePkmnDisplayWidget* highWidget = new BasePkmnDisplayWidget(groupBoxQList.at(i),highPkmn);
            BasePkmnDisplayWidget* lowWidget = new BasePkmnDisplayWidget(groupBoxQList.at(i),lowPkmn);

            //Create QLabels with BasePkmnDisplayWidgets
            QLabel* highLabel = new QLabel(tr("High:"));
            QLabel* lowLabel = new QLabel(tr("Low:"));

            //Create stat number labels (PROBABLY WILL CHANGE LATER)
            QLabel* highStat = new QLabel(QString("(%1)").arg(QString::number(high_vec[i].stat_value)));
            QLabel* lowStat = new QLabel(QString("(%1)").arg(QString::number(low_vec[i].stat_value)));

            //Separator and calculate button
            QFrame* vertLine = new QFrame(groupBoxQList.at(i));
            vertLine->setObjectName("vertLine");
            vertLine->setFrameShape(QFrame::VLine);

            //Add widgets to QGroupBoxes
            groupBoxQList.at(i)->layout()->addWidget(highLabel);
            groupBoxQList.at(i)->layout()->addWidget(highWidget);
            groupBoxQList.at(i)->layout()->addWidget(highStat);
            groupBoxQList.at(i)->layout()->addWidget(vertLine);
            groupBoxQList.at(i)->layout()->addWidget(lowLabel);
            groupBoxQList.at(i)->layout()->addWidget(lowWidget);
            groupBoxQList.at(i)->layout()->addWidget(lowStat);
            
            groupBoxQList.at(i)->layout()->setSpacing(10);
            groupBoxQList.at(i)->layout()->setMargin(5);
        }
    }

    update();
}
