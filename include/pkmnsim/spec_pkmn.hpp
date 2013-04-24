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
    /*
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

            //Class constructors (should never be called directly)
            spec_pkmn() {};
            spec_pkmn(base_pkmn::sptr b, bool i, std::string m1, std::string m2,
                      std::string m3, std::string m4, int g, int l);


            /*
             * Returns a boost::shared_ptr<spec_pkmn> of generated specific Pokémon.
             * Some values are random within bounds of specified values.
             *
             * Parameters:
             *  - identifier: this Pokemon's base Pokemon's identifier.
             *  - gen: the generation whose algorithms to use.
             *  - level: Pokémon's level.
             *  - move1: identifier for move 1.
             *  - move2: identifier for move 2.
             *  - move3: identifier for move 3.
             *  - move4: identifier for move 4.
             *  - illegal: ignore illegal stats, moves, etc.
             */
            static sptr make(std::string identifier, int gen, int level,
                             std::string move1, std::string move2,
                             std::string move3, std::string move4,
                             bool illegal);

            /*
             * Returns the base Pokémon used to generate this Pokémon.
             */
            base_pkmn::sptr get_base_pkmn(void) {return base;}

            /*
             * Returns the Pokémon's nickname.
             */
            std::string get_nickname(void) {return nickname;}

            /*
             * Returns the Pokémon's level.
             */
            int get_level(void) {return level;}

            /*
             * Returns the Pokémon's stats.
             *
             * Query stats as follows:
             *  - HP: dict_name["HP"]
             *  - Attack: dict_name["ATK"]
             *  - Defense: dict_name["DEF"]
             *  - Speed: dict_name["SPD"]
             *  - Special: dict_name["SPCL"] (Gen 1 only)
             *  - Special Attack: dict_name["SATK"] (Gen 2-5 only)
             *  - Special Defense: dict_name["SDEF"] (Gen 2-5 only)
             */
            virtual dict<std::string, int> get_stats(void) = 0;

            /*
             * Returns the Pokémon's individual values.
             * NOTE: These values are sometimes randomly generated.
             *
             * Query stats as follows:
             *  - HP: dict_name["HP"]
             *  - Attack: dict_name["ATK"]
             *  - Defense: dict_name["DEF"]
             *  - Speed: dict_name["SPD"]
             *  - Special: dict_name["SPCL"] (Gen 1 only)
             *  - Special Attack: dict_name["SATK"] (Gen 2-5 only)
             *  - Special Defense: dict_name["SDEF"] (Gen 2-5 only)
             */
            virtual dict<std::string, int> get_IVs(void) = 0;

            /*!
             * Returns the Pokémon's effort values.
             * NOTE: These values are sometimes randomly generated.
             *
             * Query stats as follows:
             *  - HP: dict_name["HP"]
             *  - Attack: dict_name["ATK"]
             *  - Defense: dict_name["DEF"]
             *  - Speed: dict_name["SPD"]
             *  - Special: dict_name["SPCL"] (Gen 1 only)
             *  - Special Attack: dict_name["SATK"] (Gen 2-5 only)
             *  - Special Defense: dict_name["SDEF"] (Gen 2-5 only)
             */
            virtual dict<std::string, int> get_EVs(void) = 0;

            /*
             * Returns the names of the Pokémon's moves.
             */
            vla<base_move::sptr> get_moves(void) {return moves;}

            /*
             * Returns a string with basic information on the Pokémon.
             */
            virtual std::string get_info(void) = 0;

            /*
             * Returns a string with all information on the Pokémon.
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

            /*
             * Reset map of volatile statuses (confusion, infatuation, etc.).
             * This map varies with the generation.
             */
            virtual void reset_volatile_status_map(void) = 0;
    };
}
#endif /* INCLUDED_SPEC_PKMN_HPP */
