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
    QHBoxLayout* hpLayout = new QHBoxLayout(this);
    QLabel* hpEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
    hpLayout->addWidget(hpEmptyLabel);
    hpLayout->setAlignment(Qt::AlignCenter);
    
    QGroupBox* attackGroupBox = new QGroupBox(tr("Attack"),this);
    attackGroupBox->setObjectName("attackGroupBox");
    QHBoxLayout* attackLayout = new QHBoxLayout(this);
    QLabel* attackEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
    attackLayout->addWidget(attackEmptyLabel);
    attackLayout->setAlignment(Qt::AlignCenter);

    QGroupBox* defenseGroupBox = new QGroupBox(tr("Defense"),this);
    defenseGroupBox->setObjectName("defenseGroupBox");
    QHBoxLayout* defenseLayout = new QHBoxLayout(this);
    QLabel* defenseEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
    defenseLayout->addWidget(defenseEmptyLabel);
    defenseLayout->setAlignment(Qt::AlignCenter);

    QGroupBox* speedGroupBox = new QGroupBox(tr("Speed"),this);
    speedGroupBox->setObjectName("defenseGroupBox");
    QHBoxLayout* speedLayout = new QHBoxLayout(this);
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
        QHBoxLayout* specialLayout = new QHBoxLayout(this);
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
        QHBoxLayout* specialAttackLayout = new QHBoxLayout(this);
        specialAttackGroupBox->setLayout(specialAttackLayout);
        QLabel* specialAttackEmptyLabel = new QLabel(tr("Press the Calculate button to show results."));
        specialAttackLayout->addWidget(specialAttackEmptyLabel);
        specialAttackLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(specialAttackGroupBox);

        QGroupBox* specialDefenseGroupBox = new QGroupBox(tr("Special Defense"),this);
        specialDefenseGroupBox->setObjectName("specialDefenseGroupBox");
        QHBoxLayout* specialDefenseLayout = new QHBoxLayout(this);
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
                                          vector<vector<stat_st> > lowest_stats_vecs)
{
    vector<stat_st> high_vec = highest_stats_vecs[generation-1];
    vector<stat_st> low_vec = lowest_stats_vecs[generation-1];

    QList<QGroupBox*> groupBoxQList = this->findChildren<QGroupBox*>();
    for(int i = 1; i <= groupBoxQList.count(); i++)
    {
        //Hacky way of getting rid of "Press the Calculate button..." QLabel
        QList<QLabel*> labelQList = groupBoxQList.at(i)->findChildren<QLabel*>();
        if(labelQList.count()) groupBoxQList.at(i)->layout()->removeWidget(labelQList.at(1));
    }
    
    /*QGroupBox* hpGroupBox = this->findChild<QGroupBox*>(QString("hpGroupBox"));
    QGroupBox* attackGroupBox = this->findChild<QGroupBox*>(QString("attackGroupBox"));
    QGroupBox* defenseGroupBox = this->findChild<QGroupBox*>(QString("defenseGroupBox"));
    QGroupBox* speedGroupBox = this->findChild<QGroupBox*>(QString("speedGroupBox"));*/
    
    update();
}