/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TRAINER_HPP
#define INCLUDED_PKMN_TRAINER_HPP

#include <string>

#include <pkmn/bag.hpp>
#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/types/pokemon_text.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    /*!
     * This class represents a <a href="http://bulbapedia.bulbagarden.net/wiki/Trainer">Pokémon trainer</a>. From
     * this class, you can access a trainer's party and bag, as well as information about them, such as
     * their name and ID.
     *
     * To instantiate a trainer shared_ptr, you can use one of two methods:
     * <pre>
     * pkmn::trainer::sptr pkmn_trainer = pkmn::trainer::make(pkmn::Games::HEART_GOLD, "Gold", pkmn::Genders::MALE);
     * pkmn::trainer::sptr pkmn_trainer = pkmn::trainer::make("Heart Gold", "Gold", "Male");
     * </pre>
     */
    class PKMN_API trainer
    {
        public:

            typedef pkmn::shared_ptr<trainer> sptr;

            /*!
             * This is the class's factory function. It returns a shared_ptr to a trainer
             * based on the given parameters.
             *
             * \param game SQLite ID of the desired game
             * \param name trainer's name
             * \param gender SQLite ID of the trainer's gender
             * \return shared pointer to instance of pkmn::trainer with described parameters
             */
            static sptr make(unsigned int game, pokemon_text name, unsigned int gender);

            /*!
             * This is the class's factory function. It takes in the name of the game this trainer
             * comes from, the trainer's name, and the trainer's gender.
             *
             * \param game name of the desired game
             * \param name trainer's name
             * \param gender trainer's gender
             * \return shared pointer to instance of pkmn::trainer with described parameters
             */
            static sptr make(std::string game, pokemon_text name, std::string gender);
            
            trainer() {};
            virtual ~trainer() {};

            /*!
             * Return the name of the game from which this trainer was created.
             *
             * Examples: "Red", "Diamond"
             *
             * \return game's name
             */
            virtual pokemon_text get_game() const = 0;

            /*!
             * Return the generation (1-6) of the game from which this trainer
             * was created.
             *
             * \return game's generation
             */
            virtual unsigned int get_generation() const = 0;

            /*!
             * Return the trainer's name.
             *
             * \return name of trainer
             */
            virtual pokemon_text get_name() const = 0;

            /*!
             * Return the amount of money the trainer currently has (0-999999).
             *
             * \return trainer's money amount
             */
            virtual unsigned int get_money() const = 0;

            /*!
             * Return the trainer's gender ("Male" or "Female").
             *
             * \return trainer's gender
             */
            virtual pokemon_text get_gender() const = 0;

            /*!
             * Return the trainer's full ID. In Generations I-II, this value can span the entire
             * range of an unsigned 16-bit number (0-65535). In Generations III-VI, this range is
             * extended to the range of an unsigned 32-bit number (0-4294967295); however, the in-game
             * value is always seen as the second 16-bits of the number.
             *
             * \return trainer's full ID (both secret and public)
             */
            virtual unsigned int get_id() const = 0;
            
            /*!
             * In Generations I-II, return the trainer's full ID. In Generations III-VI, return the ID
             * displayed in-game, which is the second half of the full ID. This number can be anywhere
             * in the range 0-65535.
             *
             * \return trainer's ID as seen in-game
             */
            virtual unsigned short get_public_id() const = 0;

            /*!
             * In Generations III-VI, there is a second half of the trainer ID that is never seen in-game.
             * It is the first half of the full trainer ID. In Generations I-II, this value is always 0.
             *
             * \return trainer's secret ID, never seen in-game
             */
            virtual unsigned short get_secret_id() const = 0;

            /*!
             * Set the trainer's name. This value must be of length 1-7, or the function will do nothing.
             *
             * \param name trainer's new name
             */
            virtual void set_name(pokemon_text name) = 0;

            /*!
             * Set the amount of money the trainer has. This value must be 0-999999, or the function will do nothing.
             *
             * \param money new amount of money
             */
            virtual void set_money(unsigned int money) = 0;

            /*!
             * Set the trainer's new gender. This value must be "Male" or "Female", or the function will do nothing.
             *
             * \param gender trainer's new gender
             */
            virtual void set_gender(pokemon_text gender) = 0;

            /*!
             * Set the trainer's new full ID. This value must be in range 0-4294967295 (the range of an unsigned 32-bit
             * number). If the trainer comes from Generation I-II and the given ID is greater than 65535, this effectively
             * sets the value to 65535.
             *
             * \param id new trainer ID
             */
            virtual void set_id(unsigned int id) = 0;

            /*!
             * Sets the trainer's new public ID, as seen in-game. This value must be in range 0-65536. For trainers from
             * Generations I-II, this is effectively the same as set_id.
             *
             * \param id trainer's new public ID
             */
            virtual void set_public_id(unsigned short id) = 0;

            /*!
             * Sets the trainer's new secret ID, which is never seen in-game. This value must be in range 0-65536.
             * For trainers from Generations I-II, this function does nothing.
             *
             * \param id trainer's new secret ID
             */
            virtual void set_secret_id(unsigned short id) = 0;

            /*!
             * Return the Pokémon at the given position in the trainer's party. The given position must be in the range
             * 1-6, or this function will return an Invalid Pokémon.
             *
             * \param pos Position of Pokémon in party to return
             * \return Pokémon in given position
             */
            virtual team_pokemon::sptr get_pokemon(unsigned int pos) = 0;

            /*!
             * Replaces the Pokémon at the given position (1-6). Currently, this will fail if the new Pokémon is not from the
             * same version group.
             *
             * \param pos position to place new Pokémon
             * \param t_pkmn new Pokémon
             */
            virtual void set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn) = 0;

            /*!
             * Removes the Pokémon at the given position. Shift any subsequent Pokémon backward.
             *
             * \param pos position of Pokémon to remove.
             */
            virtual void remove_pokemon(unsigned int pos) = 0;

            /*!
             * Places the party in the given pokemon_team vector.
             *
             * \param party reference to a pokemon_team vector
             */
            virtual void get_party(pokemon_team_t& party) = 0;

            /*!
             * Sets the party to the given pokemon_vector. Currently only takes Pokémon
             * from the same game.
             *
             * \param party reference to a pokemon_team vector
             */
            virtual void set_party(pokemon_team_t& party) = 0;

            //! Returns a pointer to the trainer's bag.
            virtual bag::sptr get_bag() const = 0;

            //! Return SQLite Database ID of game used to create this item instance
            virtual unsigned int get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_TRAINER_HPP */
