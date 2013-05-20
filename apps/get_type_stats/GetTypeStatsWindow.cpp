/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "GetTypeStatsWindow.hpp"
#include "type_stats_common.hpp"

using namespace pkmnsim;
using namespace std;

GetTypeStatsWindow::GetTypeStatsWindow(QWidget* parent): QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* typeBoxesLayout = new QHBoxLayout();
    QFormLayout* leftTypeBoxLayout = new QFormLayout();
    QFormLayout* rightTypeBoxLayout = new QFormLayout();

    TypeBox1 = new TypesComboBox();
    TypeBox2 = new TypesComboBox();

    leftTypeBoxLayout->addRow(tr("Type 1:"), TypeBox1);
    rightTypeBoxLayout->addRow(tr("Type 2:"), TypeBox2);
    
    typeBoxesLayout->addLayout(leftTypeBoxLayout);
    typeBoxesLayout->addLayout(rightTypeBoxLayout);

    mainLayout->addLayout(typeBoxesLayout);
    setLayout(mainLayout);
}
