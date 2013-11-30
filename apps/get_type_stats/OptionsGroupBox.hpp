/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_OPTIONSGROUPBOX_HPP
#define INCLUDED_OPTIONSGROUPBOX_HPP

#include <string>
#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <pkmnsim/qt4/TypesComboBox.hpp>

#include "type_stats_common.hpp"

class OptionsGroupBox: public QGroupBox
{
    Q_OBJECT

    public:
        OptionsGroupBox(QWidget* parent = 0);

    signals:
        void resultsCalculated(std::vector<std::vector<stat_st> > highest_stats_vecs,
                               std::vector<std::vector<stat_st> > lowest_stats_vecs,
                               std::vector<int> errcodes,
                               std::string type1, std::string type2);

    private slots:
        void setTypeOne(QString typeOneQString);
        void setTypeTwo(QString typeTwoQString);
        void setEvolved(int state);
        void calculateResults();

    private:
        std::string type1, type2;
        bool evolved;

};
#endif /* INCLUDED_OPTIONSGROUPBOX_HPP */
