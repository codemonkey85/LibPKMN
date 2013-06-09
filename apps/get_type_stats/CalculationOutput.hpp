/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_CALCULATIONOUTPUT_HPP
#define INCLUDED_CALCULATIONOUTPUT_HPP

#include <vector>

#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <pkmnsim/BasePkmnDisplayWidget.hpp>

#include "type_stats_common.hpp"

class CalculationOutput: public QWidget
{
    Q_OBJECT

    public:
        CalculationOutput(QWidget* parent, int gen);

    public slots:
        void getAndShowResults(std::vector<std::vector<stat_st> > highest_stats_vecs,
                               std::vector<std::vector<stat_st> > lowest_stats_vecs,
                               std::vector<int> errcodes);

    private:
        int generation;
};
#endif /* INCLUDED_CALCULATIONOUTPUT_HPP */
