/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_IO_HPP
#define INCLUDED_PKMN_IO_HPP

#include <pkmn/team_pokemon.hpp>

namespace pkmn
{
    namespace io
    {
        void export_to_3gpkm(team_pokemon::sptr t_pkmn, std::string filename);
        team_pokemon::sptr import_from_3gpkm(std::string filename);

        team_pokemon::sptr import_from_pkm(std::string filename);

        void export_to_pkm(team_pokemon::sptr t_pkmn, std::string filename);
        team_pokemon::sptr import_from_pkm(std::string filename);

        void export_to_pkx(team_pokemon::sptr t_pkmn, std::string filename);
        team_pokemon::sptr import_from_pkx(std::string filename);
        
        void export_to_pksql(team_pokemon::sptr t_pkmn, std::string filename,
                                         std::string title = "");

        team_pokemon::sptr import_from_pksql(std::string filename);
    }
}

#endif /* INCLUDED_PKMN_IO_HPP */
