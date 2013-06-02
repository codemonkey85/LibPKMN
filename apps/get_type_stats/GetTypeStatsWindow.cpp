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
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    OptionsGroupBox* optionsGroupBox = new OptionsGroupBox(this);
    QTabBar* genTabs = new QTabBar(this);
    genTabs->addTab(QString("Gen 1"));
    genTabs->addTab(QString("Gen 2"));
    genTabs->addTab(QString("Gen 3"));
    genTabs->addTab(QString("Gen 4"));
    genTabs->addTab(QString("Gen 5"));

    mainLayout->addWidget(optionsGroupBox);
    mainLayout->addWidget(genTabs);
    mainLayout->setAlignment(Qt::AlignTop);

    setLayout(mainLayout);
}
