/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
 #ifndef INCLUDED_POKEMONDISPLAYWIDGET_HPP
 #define INCLUDED_POKEMONDISPLAYWIDGET_HPP
 
 #include <pkmnsim/config.hpp>
 #include <pkmnsim/spec_pkmn.hpp>
 #include <pkmnsim/vla.hpp>
 
 #include <QChar>
 #include <QFormLayout>
 #include <QHBoxLayout>
 #include <QLabel>
 #include <QString>
 #include <QVBoxLayout>
 #include <QWidget>
 
 namespace pkmnsim
 {
    class PKMNSIM_API PokemonDisplayWidget: public QWidget
    {
        Q_OBJECT
        
        public:
            PokemonDisplayWidget(QWidget* parent = 0, spec_pkmn::sptr pkmn = *(new spec_pkmn::sptr));
            
        /*signals:
        
        slots:*/
        
        private:
            spec_pkmn::sptr s_pkmn;
    };
 }
 
 #endif /* INCLUDED_POKEMONDISPLAYWIDGET_HPP */