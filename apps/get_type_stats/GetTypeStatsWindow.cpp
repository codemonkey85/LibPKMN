/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "GetTypeStatsWindow.hpp"

#include <pkmnsim/base_pokemon.hpp>

using namespace pkmnsim;
using namespace pkmnsim::qt4;
using namespace std;

GetTypeStatsWindow::GetTypeStatsWindow(QWidget* parent): QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    OptionsGroupBox* optionsGroupBox = new OptionsGroupBox(this);
    QTabWidget* genTabs = new QTabWidget(this);
    genTabs->addTab(new CalculationOutput(this,1), QString("Gen 1"));
    genTabs->addTab(new CalculationOutput(this,2), QString("Gen 2"));
    genTabs->addTab(new CalculationOutput(this,3), QString("Gen 3"));
    genTabs->addTab(new CalculationOutput(this,4), QString("Gen 4"));
    genTabs->addTab(new CalculationOutput(this,5), QString("Gen 5"));

    //This is ugly...can this be done better?
    connect(optionsGroupBox, SIGNAL(resultsCalculated(std::vector<std::vector<stat_st2> >,
                                    std::vector<std::vector<stat_st2> >, std::vector<int>,
                                    std::string, std::string)),
            genTabs->widget(0), SLOT(getAndShowResults(std::vector<std::vector<stat_st2> >,
                                     std::vector<std::vector<stat_st2> >, std::vector<int>,
                                     std::string, std::string)));

    connect(optionsGroupBox, SIGNAL(resultsCalculated(std::vector<std::vector<stat_st2> >,
                                    std::vector<std::vector<stat_st2> >, std::vector<int>,
                                    std::string, std::string)),
            genTabs->widget(1), SLOT(getAndShowResults(std::vector<std::vector<stat_st2> >,
                                     std::vector<std::vector<stat_st2> >, std::vector<int>,
                                     std::string, std::string)));

    connect(optionsGroupBox, SIGNAL(resultsCalculated(std::vector<std::vector<stat_st2> >,
                                    std::vector<std::vector<stat_st2> >, std::vector<int>,
                                    std::string, std::string)),
            genTabs->widget(2), SLOT(getAndShowResults(std::vector<std::vector<stat_st2> >,
                                     std::vector<std::vector<stat_st2> >, std::vector<int>,
                                     std::string, std::string)));

    connect(optionsGroupBox, SIGNAL(resultsCalculated(std::vector<std::vector<stat_st2> >,
                                    std::vector<std::vector<stat_st2> >, std::vector<int>,
                                    std::string, std::string)),
            genTabs->widget(3), SLOT(getAndShowResults(std::vector<std::vector<stat_st2> >,
                                     std::vector<std::vector<stat_st2> >, std::vector<int>,
                                     std::string, std::string)));

    connect(optionsGroupBox, SIGNAL(resultsCalculated(std::vector<std::vector<stat_st2> >,
                                    std::vector<std::vector<stat_st2> >, std::vector<int>,
                                    std::string, std::string)),
            genTabs->widget(4), SLOT(getAndShowResults(std::vector<std::vector<stat_st2> >,
                                     std::vector<std::vector<stat_st2> >, std::vector<int>,
                                     std::string, std::string)));

    mainLayout->addWidget(optionsGroupBox);
    mainLayout->addWidget(genTabs);
    mainLayout->setAlignment(Qt::AlignTop);

    setLayout(mainLayout);
}
