/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <QApplication>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QVBoxLayout>

#include <pkmnsim/TypesComboBox.hpp>

#define WIN_W 500
#define WIN_H 400

class GetTypeStatsWindow: public QWidget
{
    Q_OBJECT

    public:
        GetTypeStatsWindow(QWidget* parent = 0);

    private:
        pkmnsim::TypesComboBox *TypeBox1, *TypeBox2;
};
