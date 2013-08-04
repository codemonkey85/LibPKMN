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

#include "conversions.hpp"

namespace pkmnsim
{
    class trainer_gen3impl: public trainer
    {
        public:
            trainer_gen3impl(SQLite::Database *import_db);
            trainer_gen3impl(std::string filename, int game);

        private:
            SaveParser *parser;
    };
}

#endif /* INCLUDED_TRAINER_GEN3IMPL_HPP */
