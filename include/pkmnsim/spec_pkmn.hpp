/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_SPEC_PKMN_HPP
#define INCLUDED_SPEC_PKMN_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <string>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/dict.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/pkmn_nature.hpp>
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

            typedef std::shared_ptr<spec_pkmn> sptr;

            //Class constructors (should never be called directly)
            spec_pkmn() {};
            spec_pkmn(base_pkmn::sptr b, bool i, std::string m1, std::string m2,
                      std::string m3, std::string m4, int g, int l);


            /*
             * Returns a std::shared_ptr<spec_pkmn> of generated specific Pokémon.
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
            base_pkmn::sptr get_base_pkmn(void);

            /*
             * Returns the Pokémon's nickname.
             */
            std::string get_nickname(void);

            /*
             * Returns the Pokémon's level.
             */
            int get_level(void);

            /*
             * Returns the Pokémon's ability (Gen 3-5 only)
             * NOTE: Will throw an error if Pokemon is from Generation 1-2.
             */
            virtual std::string get_ability(void) {throw std::runtime_error("Not valid in this generation.");}

            /*
             * Returns the Pokémon's gender (Gen 2-5 only)
             * NOTE; Will throw an error if Pokemon is from Generation 1.
             */
            virtual int get_gender(void) {throw std::runtime_error("Not valid in this generation.");}

            /*
             * Returns the Pokémon's nature (Gen 3-5 only).
             * NOTE: Will throw an error if Pokemon is from Generation 1-2.
             */
            virtual pkmn_nature::sptr get_nature(void) {throw std::runtime_error("Not valid in this generation.");}

            /*
             * Returns whether or not Pokemon is shiny.
             */
            bool is_shiny(void);

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
            vla<base_move::sptr> get_moves(void);
            
            /*
             * Returns the generation used to generate this Pokemon.
             */
            int get_generation(void);

            /*
             * Returns the name of the held item.
             */
            virtual std::string get_held_item(void) {throw std::runtime_error("Not valid in this generation.");}

            /*
             * Returns a string with basic information on the Pokémon.
             */
            virtual std::string get_info(void) = 0;

            /*
             * Returns a string with all information on the Pokémon.
             */
            virtual std::string get_info_verbose(void) = 0;

            /*
             * Returns the icon path.
             */
            std::string get_icon_path(void);

            /*
             * Returns the sprite path.
             */
            std::string get_sprite_path(void);

            /*
             * Functions to more easily access base_pkmn variables and functions.
             */
            std::string get_species_name(void);
            dict<int, std::string> get_types(void);
            dict<std::string, int> get_base_stats(void);
            dict<std::string, int> get_ev_yields(void);
            bool is_fully_evolved(void);
            int get_pokemon_id(void);
            int get_species_id(void);
            virtual void set_form(int form) = 0;
            virtual void set_form(std::string form) = 0;

        protected:
            base_pkmn::sptr base;
            std::string nickname;
            std::string held_item;
            int generation;
            int level;
            bool shiny;
            int HP, ATK, DEF, SPD;
            int ivHP, ivATK, ivDEF, ivSPD;
            int evHP, evATK, evDEF, evSPD;
            dict<std::string, int> volatile_status_map;
            int nonvolatile_status;
            vla<base_move::sptr> moves;
            int num_moves;
            std::string icon_path, sprite_path;

            virtual int get_hp_from_iv_ev() = 0;
            virtual int get_stat_from_iv_ev(std::string, int, int) = 0; //Others share common algorithm

            /*
             * Reset map of volatile statuses (confusion, infatuation, etc.).
             * This map varies with the generation.
             */
            virtual void reset_volatile_status_map(void) = 0;

            //Used by sub-classes, but needed for friend classes to access values
            //These are otherwise only used by appropriate subclasses
            int SPCL, SATK, SDEF;
            int ivSPCL, ivSATK, ivSDEF;
            int evSPCL, evSATK, evSDEF;
            int gender;
            pkmn_nature::sptr nature;
            std::string ability;

            friend class trainer;
            friend class trainer_gen3impl;
            friend class trainer_gen4impl;
    };
}
#endif /* INCLUDED_SPEC_PKMN_HPP */
