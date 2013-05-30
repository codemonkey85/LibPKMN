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
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/vla.hpp>
#include <stdexcept>
#include <string>
#include <vector>

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
            game_save(std::string filename);

            static sptr make(std::string filename, int game_group);

            virtual std::string get_trainer_name(void) = 0;

            virtual void get_party(std::vector<spec_pkmn::sptr>& belt_pkmn_vec) = 0;

            //Better way to do this?
            virtual void get_boxes(std::vector<std::vector<spec_pkmn::sptr> >& boxes_vec) = 0;

        protected:

            std::string trainer_name;
            std::string trainer_id; //should be int?
            std::vector<spec_pkmn::sptr> party; 
            std::vector<std::vector<spec_pkmn::sptr> > boxes;
    };
}

#endif /* INCLUDED_GAME_SAVE_HPP */
