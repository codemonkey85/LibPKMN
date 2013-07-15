/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TRAINER_HPP
#define INCLUDED_TRAINER_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <string>
#include <vector>

#include <pkmnsim/config.hpp>
#include <pkmnsim/spec_pkmn.hpp>

namespace pkmnsim
{
    /*
     * Trainer Class
     *
     * This class takes in a save file or SQLite3 database created by
     * PKMNsim and generates an object with all information corresponding
     * to the source's trainer. The class stores the trainer's name,
     * ID, amount of money, and team.
     *
     * NOTE: Only Generation 3 currently supported.
     */

    class PKMNSIM_API trainer
    {
        public:

            typedef std::shared_ptr<trainer> sptr;

            //Class constructor (should never be called directly)
            trainer(void);

            /*
             * Returns a std::shared_ptr<trainer>of specified trainer.
             * If filename is a valid SQLite database, it imports
             * all values from the database. Otherwise, it attempts
             * to interpret the file as a game save that corresponds
             * to the game specified in the game parameter.
             *
             * Parameters:
             *  - filename: path of SQLite database or save file
             *  - game: Games enum value
             */
            static sptr make(std::string filename, int game);

            /*
             * Returns the trainer's amount of money.
             */
            int get_money(void);

            /*
             * Return a vector of spec_pkmn with the trainer's team.
             *
             * Parameters:
             *  - party_vec: A reference to the vector in which to place the party.
             */
            void get_party(std::vector<spec_pkmn::sptr>& party_vec);

            /*
             * Return a vector of spec_pkmn with the trainer's team.
             */
            std::vector<spec_pkmn::sptr> get_party_vec(void);

            /*
             * Returns the trainer's name.
             */
            std::string get_trainer_name(void);

            /*
             * Exports the trainer's values to a SQLite database.
             *
             * Parameters:
             *  - filename: the name of the file in which to export the values
             */
            virtual void export_to_file(std::string filename);

        protected:
            int money, from_game, gender;
            std::string trainer_name, trainer_id;
            std::vector<spec_pkmn::sptr> party;
    };
}

#endif /* INCLUDED_TRAINER_HPP */
