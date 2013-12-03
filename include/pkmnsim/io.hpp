/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_IO_HPP
#define INCLUDED_PKMNSIM_IO_HPP

#include <pkmnsim/config.hpp>
#include <pkmnsim/team_pokemon.hpp>

namespace pkmnsim
{
    namespace io
    {
        void PKMNSIM_API export_to_pkm(team_pokemon::sptr t_pkmn, std::string filename);

        team_pokemon::sptr PKMNSIM_API import_from_pkm(std::string filename);
        
        void PKMNSIM_API export_to_pkx(team_pokemon::sptr t_pkmn, std::string filename);
        
        team_pokemon::sptr PKMNSIM_API import_from_pkx(std::string filename);

        void PKMNSIM_API export_to_pksql(team_pokemon::sptr t_pkmn, std::string filename);

        //team_pokemon::sptr PKMNSIM_API import_from_pksql(std::string filename);
    }
}

#endif /* INCLUDED_PKMNSIM_IO_HPP */
