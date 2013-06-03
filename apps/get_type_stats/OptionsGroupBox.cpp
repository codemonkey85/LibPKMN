/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "OptionsGroupBox.hpp"

using namespace pkmnsim;
using namespace std;

OptionsGroupBox::OptionsGroupBox(QWidget* parent): QGroupBox(parent)
{
    //Initialize private variables
    type1 = "Normal";
    type2 = "Normal";
    evolved = false;
    lax = false;

    setTitle(QString("Options"));

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* choicesLayout = new QVBoxLayout();
    QHBoxLayout* typesLayout = new QHBoxLayout();
    QHBoxLayout* checkBoxesLayout = new QHBoxLayout();
    QVBoxLayout* buttonLayout = new QVBoxLayout();

    //typesLayout
    QLabel* typesLabel = new QLabel(tr("Input type(s):"),this);
    TypesComboBox* type1ComboBox = new TypesComboBox(this,5);
    TypesComboBox* type2ComboBox = new TypesComboBox(this,5);
    typesLayout->addWidget(typesLabel);
    typesLayout->addWidget(type1ComboBox);
    typesLayout->addWidget(type2ComboBox);

    //checkBoxesLayout
    QCheckBox* evolvedCheckBox = new QCheckBox(tr("Only fully evolved Pokemon?"),this);
    QCheckBox* laxCheckBox = new QCheckBox(tr("If single type specified, any combo?"), this);
    checkBoxesLayout->addWidget(evolvedCheckBox);
    checkBoxesLayout->addWidget(laxCheckBox);

    //Separator and calculate button
    QFrame* vertLine = new QFrame(this);
    vertLine->setFrameShape(QFrame::VLine);
    QPushButton* calculateButton = new QPushButton(tr("Calculate"),this);

    //Setting layer hierarchy
    choicesLayout->addLayout(typesLayout);
    choicesLayout->addLayout(checkBoxesLayout);
    mainLayout->addLayout(choicesLayout);
    mainLayout->addWidget(vertLine);
    mainLayout->addWidget(calculateButton);

    setLayout(mainLayout);
    setFixedSize(800,100);
}

//Private slots to set private variables
void OptionsGroupBox::setTypeOne(QString typeOneQString) {type1 = typeOneQString.toStdString();}
void OptionsGroupBox::setTypeTwo(QString typeTwoQString) {type2 = typeTwoQString.toStdString();}
void OptionsGroupBox::setEvolved(int state) {evolved = state;}
void OptionsGroupBox::setLax(int state) {lax = state;}

void OptionsGroupBox::calculateResults()
{
    vector<vector<stat_st> > highest_stats_vecs, lowest_stats_vecs;

    for(int i = 0; i < 5; i++)
    {
        vector<stat_st> high_vec, low_vec;
        sort_pokemon_by_stats(type1, type2, high_vec, low_vec, i, lax, evolved);
        highest_stats_vecs.push_back(high_vec);
        lowest_stats_vecs.push_back(low_vec);
    }
    emit resultsCalculated(highest_stats_vecs, lowest_stats_vecs);
}
