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
