/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "GetTypeStatsWindow.hpp"

#include <pkmnsim/base_pkmn.hpp>

using namespace pkmnsim;
using namespace std;

GetTypeStatsWindow::GetTypeStatsWindow(QWidget* parent): QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    OptionsGroupBox* optionsGroupBox = new OptionsGroupBox(this);
    QTabWidget* genTabs = new QTabWidget(this);
    genTabs->addTab(new QWidget(), QString("Gen 1"));
    genTabs->addTab(new QWidget(), QString("Gen 2"));
    genTabs->addTab(new QWidget(), QString("Gen 3"));
    genTabs->addTab(new QWidget(), QString("Gen 4"));
    genTabs->addTab(new QWidget(), QString("Gen 5"));

    mainLayout->addWidget(optionsGroupBox);
    mainLayout->addWidget(genTabs);
    mainLayout->setAlignment(Qt::AlignTop);

    setLayout(mainLayout);
}
