/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef SPEC_PKMN_HPP
#define SPEC_PKMN_HPP

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/pkmn_nature.hpp>

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

            typedef boost::shared_ptr<spec_pkmn> sptr;

            /*!
             * Make a new specific Pokémon with random values.
             * \param pkmn This Pokémon's base Pokémon.
             * \param gen The generation whose algorithms to use.
             * \return A new specific Pokémon shared pointer
             */
            static sptr make(std::string identifier, int gen);

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
            virtual std::map<std::string, int> get_stats(void) = 0;

            /*!
             * Get the Pokémon's individual values.
             * Note: These values are sometimes randomly generated.
             * \return List of ints with IVs
             */
            virtual std::map<std::string, int> get_IVs(void) = 0;

            /*!
             * Get the Pokémon's effort values.
             * Note: These values are sometimes randomly generated.
             * \return List of ints with EVs
             */
            virtual std::map<std::string, int> get_EVs(void) = 0;

            /*
             * Get the names of the Pokémon's moves.
             * \return List of strings with names of Pokémon's moves.
             */
            std::string * get_moves(void)
            {
                std::string * moves = new std::string[4];
                moves[0] = move1;
                moves[1] = move2;
                moves[2] = move3;
                moves[3] = move4;

                return moves;
            }

            /*
             * Get the PP of the Pokémon's moves.
             * \return List of ints with PP of the Pokémon's moves.
             */
            std::map<std::string, int> get_move_PPs(void)
            {
                std::map<std::string, int> movePPs;
                movePPs[move1] = move1PP;
                movePPs[move2] = move2PP;
                movePPs[move3] = move3PP;
                movePPs[move4] = move4PP;

                return movePPs;
            }

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
            pkmnsim::base_pkmn::sptr base;
            std::string nickname;
            int level;
            int HP, ATK, DEF, SPD;
            int ivHP, ivATK, ivDEF, ivSPD;
            int evHP, evATK, evDEF, evSPD;
            std::map<std::string, int> volatile_status_map;
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
            std::string move1, move2, move3, move4; //Will change moves to their own classes
            int move1PP, move2PP, move3PP, move4PP;

            virtual int get_hp_from_iv_ev() = 0;
            virtual int get_stat_from_iv_ev(std::string, int, int) = 0; //Others share common algorithm

            /*!
             * Reset map of volatile statuses (confusion, infatuation, etc.).
             * This map varies with the generation.
             */
            virtual void reset_volatile_status_map(void) = 0;
    };

}
#endif
