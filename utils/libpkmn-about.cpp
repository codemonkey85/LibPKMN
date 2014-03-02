/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <qapplication.h>

#include <pkmn/qt/AboutMessageBox.hpp>

int main(int argc, char **argv)
{
    QApplication::setStyle("cleanlooks");
    QApplication::setColorSpec(QApplication::CustomColor);
    QApplication app(argc,argv);

    pkmn::qt::AboutMessageBox box;
    box.setWindowTitle("LibPKMN - About");
    box.show();

    return app.exec();
}
