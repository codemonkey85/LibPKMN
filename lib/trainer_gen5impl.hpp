/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TRAINER_GEN5IMPL_HPP
#define INCLUDED_TRAINER_GEN5IMPL_HPP

#include "sqlitecpp/SQLiteCPP.h"

#include <pkmnsim/trainer.hpp>

#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g5_sqlite.h>

namespace pkmnsim
{
    class PKMNSIM_API trainer_gen5impl: public trainer
    {
        public:
            trainer_gen5impl(SQLite::Database *import_db);
            trainer_gen5impl(std::string filename, int game);

        private:
            bw2sav_obj* sav;
            pokemon_obj* pkm;
    };
}

#endif /* INCLUDED_TRAINER_GEN5IMPL_HPP */
