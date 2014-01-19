/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <qapplication.h>

#include <pkmn/qt4/AboutMessageBox.hpp>

int main(int argc, char **argv)
{
    QApplication::setStyle("cleanlooks");
    QApplication::setColorSpec(QApplication::CustomColor);
    QApplication app(argc,argv);

    pkmn::qt4::AboutMessageBox box;
    box.setWindowTitle("LibPKMN - About");
    box.show();

    return app.exec();
}
