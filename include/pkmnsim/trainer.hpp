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
    class PKMNSIM_API trainer
    {
        public:

            typedef std::shared_ptr<trainer> sptr;

            //Class constructors (should never be called directly)
            trainer(void);

            static sptr make(std::string filename, int gen);

            int get_money(void);

            void get_party(std::vector<spec_pkmn::sptr>& party_vec);

            std::vector<spec_pkmn::sptr> get_party_vec(void);

            std::string get_trainer_name(void);

        protected:
            int money, gender;
            std::string trainer_name, trainer_id;
            std::vector<spec_pkmn::sptr> party;
    };
}

#endif /* INCLUDED_TRAINER_HPP */
