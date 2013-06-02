/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "GetTypeStatsWindow.hpp"
#include "type_stats_common.hpp"

#include <pkmnsim/base_pkmn.hpp>

using namespace pkmnsim;
using namespace std;

GetTypeStatsWindow::GetTypeStatsWindow(QWidget* parent): QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* typeBoxesLayout = new QHBoxLayout();
    QFormLayout* leftTypeBoxLayout = new QFormLayout();
    QFormLayout* rightTypeBoxLayout = new QFormLayout();
    QHBoxLayout* hpLayout = new QHBoxLayout();
    QFormLayout* hpHighLayout = new QFormLayout();
    QFormLayout* hpLowLayout = new QFormLayout();

    TypesComboBox* TypeBox1 = new TypesComboBox();
    TypesComboBox* TypeBox2 = new TypesComboBox();

    base_pkmn::sptr pkmn1 = base_pkmn::make("Bulbasaur",5,false);
    base_pkmn::sptr pkmn2 = base_pkmn::make("Squirtle",3,false);
    BasePkmnDisplayWidget* Pkmn1Widget = new BasePkmnDisplayWidget(this,pkmn1);
    BasePkmnDisplayWidget* Pkmn2Widget = new BasePkmnDisplayWidget(this,pkmn2);

    leftTypeBoxLayout->addRow(tr("Type 1:"), TypeBox1);
    rightTypeBoxLayout->addRow(tr("Type 2:"), TypeBox2);
    
    hpHighLayout->addRow(tr("High:"), Pkmn1Widget);
    hpLowLayout->addRow(tr("Low:"), Pkmn2Widget);

    hpLayout->addLayout(hpHighLayout);
    hpLayout->addLayout(hpLowLayout);

    typeBoxesLayout->addLayout(leftTypeBoxLayout);
    typeBoxesLayout->addLayout(rightTypeBoxLayout);

    mainLayout->addLayout(typeBoxesLayout);
    mainLayout->addLayout(hpLayout);
    setLayout(mainLayout);
}
