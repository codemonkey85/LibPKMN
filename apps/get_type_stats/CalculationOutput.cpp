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
}

//Slot to call when results are calculated by OptionsGroupBox
void CalculationOutput::getAndShowResults(vector<vector<stat_st> > highest_stats_vecs,
                                          vector<vector<stat_st> > lowest_stats_vecs)
{
    high_vecs = highest_stats_vecs;
    low_vecs = lowest_stats_vecs;

    //At this point, Pokemon will already be sorted by stats
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QGroupBox* hpGroupBox = new QGroupBox(tr("HP"),this);
    QHBoxLayout* hpLayout = new QHBoxLayout(this);

    hpGroupBox->setLayout(hpLayout);
    mainLayout->addWidget(hpGroupBox);
    setLayout(mainLayout);
}
