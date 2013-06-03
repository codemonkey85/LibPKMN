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
    QHBoxLayout* hpLayout = new QHBoxLayout(this);

    QGroupBox* attackGroupBox = new QGroupBox(tr("Attack"),this);
    QHBoxLayout* attackLayout = new QHBoxLayout(this);

    QGroupBox* defenseGroupBox = new QGroupBox(tr("Defense"),this);
    QHBoxLayout* defenseLayout = new QHBoxLayout(this);

    QGroupBox* speedGroupBox = new QGroupBox(tr("Speed"),this);
    QHBoxLayout* speedLayout = new QHBoxLayout(this);

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
        QHBoxLayout* specialLayout = new QHBoxLayout(this);
        specialGroupBox->setLayout(specialLayout);
        mainLayout->addWidget(specialGroupBox);
    }
    else
    {
        QGroupBox* specialAttackGroupBox = new QGroupBox(tr("Special Attack"),this);
        QHBoxLayout* specialAttackLayout = new QHBoxLayout(this);
        specialAttackGroupBox->setLayout(specialAttackLayout);
        mainLayout->addWidget(specialAttackGroupBox);

        QGroupBox* specialDefenseGroupBox = new QGroupBox(tr("Special Defense"),this);
        QHBoxLayout* specialDefenseLayout = new QHBoxLayout(this);
        specialDefenseGroupBox->setLayout(specialDefenseLayout);
        mainLayout->addWidget(specialDefenseGroupBox);
    }
    
    setLayout(mainLayout);
}

//Slot to call when results are calculated by OptionsGroupBox
void CalculationOutput::getAndShowResults(vector<vector<stat_st> > highest_stats_vecs,
                                          vector<vector<stat_st> > lowest_stats_vecs)
{
    high_vecs = highest_stats_vecs;
    low_vecs = lowest_stats_vecs;
    
    update();
}
