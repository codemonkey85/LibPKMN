/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_SPEC_PKMN_HPP
#define INCLUDED_SPEC_PKMN_HPP

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/dict.hpp>
#include <pkmnsim/vla.hpp>

namespace pkmnsim
{
    /*!
     * Specific Pokémon class
     *
     * This class generates an object that represents a specific
     * Pokémon, with its own stats, effort values, and individual
     * values.
     *
     * This class depends on two values: a base_pkmn, which
     * provides the base stats for the Pokémon, gender ratios, etc., 
     * and gen, which tells the class what algorithms to use to
     * determine its specific values.
     */
    class PKMNSIM_API spec_pkmn
    {
        public:

            spec_pkmn() {};
            spec_pkmn(base_pkmn::sptr b, bool i, std::string m1, std::string m2,
                      std::string m3, std::string m4, int g, int l);

            typedef boost::shared_ptr<spec_pkmn> sptr;

            /*!
             * Make a new specific Pokemon with some specified values.
             * Other values are random within bounds of specified values.
             * \param identifier This Pokemon's base Pokemon's identifier.
             * \param gen The generation whose algorithms to use.
             * \param level Pokemon's level.
             * \param move1 Identifier for move 1.
             * \param move2 Identifier for move 2.
             * \param move3 Identifier for move 3.
             * \param move4 Identifier for move 4.
             * \param illegal Ignore illegal stats, moves, etc.
             * \return A new specific Pokemon shared pointer
             */
            static sptr make(std::string identifier, int gen, int level,
                             std::string move1, std::string move2,
                             std::string move3, std::string move4,
                             bool illegal);

            /*!
             * Get the base Pokémon used to generate this Pokémon.
             * \return Base Pokémon shared pointer
             */
            base_pkmn::sptr get_base_pkmn(void) {return base;}

            /*!
             * Get the Pokémon's nickname.
             * \return String with Pokémon's nickname
             */
            std::string get_nickname(void) {return nickname;}

            /*!
             * Get the Pokémon's level.
             * \return Int with Pokémon's level
             */
            int get_level(void) {return level;}

            /*!
             * Get the Pokémon's stats.
             * \return List of ints with stats
             */
            virtual dict<std::string, int> get_stats(void) = 0;

            /*!
             * Get the Pokémon's individual values.
             * Note: These values are sometimes randomly generated.
             * \return List of ints with IVs
             */
            virtual dict<std::string, int> get_IVs(void) = 0;

            /*!
             * Get the Pokémon's effort values.
             * Note: These values are sometimes randomly generated.
             * \return List of ints with EVs
             */
            virtual dict<std::string, int> get_EVs(void) = 0;

            /*
             * Get the names of the Pokémon's moves.
             * \return List of strings with names of Pokémon's moves.
             */
            vla<base_move::sptr> get_moves(void) {return moves;}

            /*!
             * Get a string with basic information on the Pokémon.
             * \return String with basic Pokémon info
             */
            virtual std::string get_info(void) = 0;

            /*!
             * Get a string with all information on the Pokémon.
             * \return String with all Pokémon info
             */
            virtual std::string get_info_verbose(void) = 0;


        protected:
            base_pkmn::sptr base;
            std::string nickname;
            int level;
            int HP, ATK, DEF, SPD;
            int ivHP, ivATK, ivDEF, ivSPD;
            int evHP, evATK, evDEF, evSPD;
            dict<std::string, int> volatile_status_map;
            std::string nonvolatile_status;
            /*
                Nonvolatile Status: only one of the following:
                OK  = None
                BRN = Burn
                FRZ = Freeze
                PRZ = Paralysis
                PSN = Poison
                BP# = Bad Poison # (number of turns)
                SLP = Sleep
            */
            vla<base_move::sptr> moves;
            int num_moves;

            virtual int get_hp_from_iv_ev() = 0;
            virtual int get_stat_from_iv_ev(std::string, int, int) = 0; //Others share common algorithm

            /*!
             * Reset map of volatile statuses (confusion, infatuation, etc.).
             * This map varies with the generation.
             */
            virtual void reset_volatile_status_map(void) = 0;
    };

    int get_pkmn_id(base_pkmn::sptr);
    int get_species_id(base_pkmn::sptr);
}
#endif
