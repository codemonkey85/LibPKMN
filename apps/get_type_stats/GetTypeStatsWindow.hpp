/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <QApplication>
#include <QCheckBox>
#include <QFrame>
#include <QMenu>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabBar>
#include <QVBoxLayout>

#include <pkmnsim/BasePkmnDisplayWidget.hpp>
#include <pkmnsim/TypesComboBox.hpp>

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
