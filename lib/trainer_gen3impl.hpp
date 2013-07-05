/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TRAINER_GEN3IMPL_HPP
#define INCLUDED_TRAINER_GEN3IMPL_HPP

#include "sqlitecpp/SQLiteCPP.h"

#include <pkmnsim/trainer.hpp>

#include <pokehack/SaveParser.h>

namespace pkmnsim
{
    class PKMNSIM_API trainer_gen3impl: public trainer
    {
        public:
            trainer_gen3impl(SQLite::Database *import_db);
            trainer_gen3impl(std::string filename, int game);

            int get_money();

            void get_party(std::vector<spec_pkmn::sptr>& party_vec);

            std::string get_trainer_name();

            void export_to_file(std::string filename);

        private:
            SaveParser *parser;

            spec_pkmn::sptr convert_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                 pokemon_attacks_t* pkmn_a_t,
                                                 pokemon_effort_t* pkmn_e_t,
                                                 pokemon_misc_t* pkmn_m_t,
                                                 pokemon_growth_t* pkmn_g_t);
    };
}

#endif /* INCLUDED_TRAINER_GEN3IMPL_HPP */
