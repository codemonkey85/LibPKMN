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

    mainLayout->addWidget(optionsGroupBox, Qt::AlignTop);

    setLayout(mainLayout);
}
