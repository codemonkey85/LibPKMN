/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <qapplication.h>

#include "GetTypeStatsWindow.hpp"

int main( int argc, char **argv )
{
	QApplication::setColorSpec( QApplication::CustomColor );
	QApplication app(argc,argv);

	GetTypeStatsWindow w;
	w.resize(WIN_W, WIN_H);
    w.setWindowTitle("Get Type Stats");

	w.show();

	return app.exec();
}
