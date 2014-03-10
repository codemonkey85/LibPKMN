/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_MOVE_HPP
#define INCLUDED_PKMN_MOVE_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    /*!
     * The move class represents a <a href="http://bulbapedia.bulbagarden.net/wiki/Move">Pokémon move</a>.
     * The factory function takes in a move name/ID and a game name/ID, and returns
     * a class instance from which any information can be retrieved, from battle
     * information such as base PP to other miscellaneous information like the move's
     * in-game description.
     *
     * To instantiate a move shared_ptr, you can use one of two methods:
     * <pre>
     * pkmn::move::sptr pkmn_move = pkmn::move::make("Tackle", "Diamond");
     * pkmn::move::sptr pkmn_move = pkmn::move::make(pkmn::Moves::TACKLE, pkmn::Games::DIAMOND);
     * </pre>
     */
    class PKMN_API move
    {
        public:

            typedef pkmn::shared_ptr<move> sptr;

            /*!
             * This is the class's factory function. It takes in the SQLite database ID for the game
             * and move this class should describe.
             *
             * \return shared pointer to instance of pkmn::move with described parameters
             */
            static sptr make(unsigned int id, unsigned int game);

            /*!
             * This is the class's factory function. It takes in the name of the game
             * and move this class should describe.
             *
             * \return shared pointer to instance of pkmn::move with described parameters
             */
            static sptr make(std::string name, std::string game);

            //! Class constructor (use factory function instead)
            move() {};
            virtual ~move() {};

            //! Return the name of the item from which this move was created (ex. "Red", "Diamond").
            virtual std::string get_game() const = 0;

            //! Return the generation of the item from which this move was created (1-6).
            virtual unsigned int get_generation() const = 0;

            //! Return the move's name.
            virtual std::string get_name() const = 0;

            //! Return the move's description (varies between games).
            virtual std::string get_description() const = 0;

            //! Return the move's type.
            /*!
             * Return the move's type.
             *
             * NOTE: There are eight moves for which the type differs between generations:
             * Bite, Charm, Curse, Gust, Karate Chop, Moonlight, Sand Attack, Sweet Kiss
             * (source: <a href="http://bulbapedia.bulbagarden.net/wiki/Curse_%28move%29#Trivia">Bulbapedia</a>).
             *
             * \return move's type
             */
            virtual std::string get_type() const = 0;

            //! Return the move's base attack power.
            /*!
             * Return the move's base power. This value, combined with stats from the attacker
             * and defender, is used to determine how much damage the move inflicts. For moves
             * that only involve status effects, this function will return 0.
             *
             * NOTE: For many moves, this value changes between generations.
             *
             * \return move's base power
             */
            virtual unsigned int get_base_power() const = 0;

            //! Return the move's base max PP.
            /*!
             * Return the move's base <a href="http://bulbapedia.bulbagarden.net/wiki/Power_points">Power Points (PP)</a>
             * amount, without the usage of PP Up or PP Max to increase the value.
             *
             * \return move's base PP value
             */
            virtual unsigned int get_base_pp() const = 0;

            //! Return the move's damage class ("Physical", "Special", or "Status").
            /*!
             * Return the move's damage class (Physical, Special, or Status). Physical moves are generally moves
             * that make contact with the target, while Special moves are generally projectile or beam-based.
             * Status moves inflict no direct damage but can cause both volatile status conditions (ex. Confusion)
             * or non-volatile status conditions (ex. Paralysis).
             *
             * In Generations 1-3, the distinction between Physical vs. Special moves was entirely dependent on
             * the move's type. For example, all directly damaging Fire-type moves were of the Special class.
             * However, from Generation 4 onward, this value was move-specific; for example, Fire Punch changed
             * from a Special move to a Physical move, while Flamethrower remained a Special move.
             *
             * \returns move's damage class
             */
            virtual std::string get_move_damage_class() const = 0;

            //! Return the move's accuracy (0.0-100.0).
            /*!
             * Return the move's accuracy (0.0-100.0), before any modifier moves/items are applied. For
             * example, Hydro Pump's accuracy is 80% (source: <a href="http://bulbapedia.bulbagarden.net/wiki/Hydro_pump">Bulbapedia</a>),
             * so for this move, the function would return 80.0. For moves with variable accuracy,
             * this function will return 0.0.
             *
             * \return move's base accuracy
             */
            virtual double get_base_accuracy() const = 0;

            //! Return the move's effect (ex. "Burn", "Confusion").
            virtual std::string get_effect() const = 0;

            //! Return the probability of the move's effect occurring (0.0-100.0).
            /*!
             * Return the probability (0.0-100.0) of the chance given by get_effect() occurring.
             * If the move has no associated status, this function will return 0.0.
             *
             * \return move's effect probability
             */
            virtual double get_effect_chance() const = 0;

            //! Return the move's <a href="http://bulbapedia.bulbagarden.net/wiki/Priority">priority</a> (-8 - 8).
            virtual int get_priority() const = 0;

            //! Return the move's target.
            /*!
             * In Double Battles and Triple Battles, moves can target all opponents, some opponents, allies,
             * or some combination thereof. This function returns a string describing this target or group
             * of targets.
             *
             * \return move's target
             */
            virtual std::string get_target() const = 0;

            //! Return move's SQLite database ID
            virtual unsigned int get_move_id() const = 0;

            //! Return move's target SQLite database ID
            virtual unsigned int get_target_id() const = 0;

            //! Return SQLite database ID of game used to create this move instance
            virtual unsigned int get_game_id() const = 0;

            //! Return SQLite database ID of move's damage class (Physical, Special, Status)
            virtual unsigned int get_move_damage_class_id() const = 0;

            //! Return SQLite database ID of move's base effect
            virtual unsigned int get_effect_id() const = 0;
    };

    //Related typedefs
    typedef std::vector<move::sptr> moveset_t;
}

#endif /* INCLUDED_PKMN_MOVE_HPP */
