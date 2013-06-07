/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_HPP
#define INCLUDED_GAME_SAVE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <pkmnsim/config.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/vla.hpp>

namespace pkmnsim
{
    /*
     * Game Save Class
     */
    class PKMNSIM_API game_save
    {
        public:

            typedef std::shared_ptr<game_save> sptr;

            //Class constructors (should never be called directly)
            game_save(void) {};
            game_save(std::string filename, int gen);

            static sptr make(std::string filename, int game_group);

            std::string get_trainer_name(void);

            void get_party(std::vector<spec_pkmn::sptr>& party_pkmn_vec);

            //Better way to do this?
            //virtual void get_boxes(std::vector<std::vector<spec_pkmn::sptr> >& boxes_vec) = 0;

        protected:

            std::string trainer_name;
            std::string trainer_id; //should be int?
            std::vector<spec_pkmn::sptr> party; 
            std::vector<std::vector<spec_pkmn::sptr> > boxes;
    };
}

#endif /* INCLUDED_GAME_SAVE_HPP */
