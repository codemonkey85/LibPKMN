/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GETTYPESTATSWINDOW_HPP
#define INCLUDED_GETTYPESTATSWINDOW_HPP

#include <QApplication>
#include <QCheckBox>
#include <QFrame>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMenu>
#include <QPushButton>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include <pkmn/qt4/BasePokemonDisplayWidget.hpp>
#include <pkmn/qt4/TypesComboBox.hpp>

#include "CalculationOutput.hpp"
#include "OptionsGroupBox.hpp"

#define WIN_W 800
#define WIN_H 600

class GetTypeStatsWindow: public QWidget
{
    Q_OBJECT

    public:
        GetTypeStatsWindow(QWidget* parent = 0);

    private:
};

#endif /* INCLUDED_GETTYPESTATSWINDOW_HPP */
