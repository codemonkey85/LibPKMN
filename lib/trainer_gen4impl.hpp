/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TRAINER_GEN4IMPL_HPP
#define INCLUDED_TRAINER_GEN4IMPL_HPP

#include "sqlitecpp/SQLiteCPP.h"

#include <pkmnsim/trainer.hpp>

#include <pokelib/pokelib.h>

namespace pkmnsim
{
    class trainer_gen4impl: public trainer
    {
        public:
            trainer_gen4impl(SQLite::Database *import_db);
            trainer_gen4impl(std::string filename, int game);

        private:
            PokeLib::Save *save;
    };
}

#endif /* INCLUDED_TRAINER_GEN4IMPL_HPP */
