/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("libpkmn_java_swig");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}

%typemap(javacode) pkmn::pokemon_text %{
    public char charAt(int index) {
        return toString().charAt(index);
    }

    @Override public boolean equals(Object other) {
        return (toString().equals(((PokemonText)other).toString()));
    }

    public boolean isEmpty() {
        return toString().isEmpty();
    }

    @Override public String toString() {
        return const_char();
    }
%}

/*
 * Macro for dict templates to avoid individual javacode declarations for HashMaps
 */
%define LIBPKMN_JAVA_DICT(dict_name, ctype1, ctype2, javatype1, javatype2, vec1, vec2)
    %typemap(javacode) pkmn::dict<ctype1, ctype2> %{
        public java.util.HashMap<javatype1, javatype2> toHashMap() {
            java.util.HashMap<javatype1, javatype2> output = new java.util.HashMap<javatype1, javatype2>();

            vec1 key_vec = KeySet();
            vec2 val_vec = values();

            for(int i = 0; i < key_vec.size(); i++)
            {
                output.put(key_vec.get(i), val_vec.get(i));
            }

            return output;
        }
    %}

    %rename(put) pkmn::dict<ctype1, ctype2>::insert;
    %rename(containsKey) pkmn::dict<ctype1, ctype2>::has_key;
    %rename(remove) pkmn::dict<ctype1, ctype2>::erase;
    %rename(KeySet) pkmn::dict<ctype1, ctype2>::keys;
    %rename(values) pkmn::dict<ctype1, ctype2>::vals;

    %template(dict_name) pkmn::dict<ctype1, ctype2>;
%enddef

%extend pkmn::nature {
    double at(std::string key) {return (*self)[key];}
    double at(unsigned int key) {return (*self)[key];}
};

/*
 * Add native String functions to classes to avoid the necessity of
 * PokemonText. The documentation will not show the functions with
 * PokemonText parameters.
 */
%extend pkmn::bag {
    void addItem(std::wstring input, unsigned int amount)
    {
        pkmn::pokemon_text item_name = pkmn::pokemon_text(input);
        self->add_item(item_name, amount);
    }

    void removeItem(std::wstring input, unsigned int amount)
    {
        pkmn::pokemon_text item_name = pkmn::pokemon_text(input);
        self->remove_item(item_name, amount);
    }

    unsigned int getItemAmount(std::wstring input)
    {
        pkmn::pokemon_text item_name = pkmn::pokemon_text(input);
        return self->get_item_amount(item_name);
    }
};
%extend pkmn::game_save {
    std::wstring getTrainerName() const
    {
        return self->get_trainer_name().std_wstring();
    }

    void setTrainerName(std::wstring input)
    {
        pkmn::pokemon_text trainer_name(input);
        self->set_trainer_name(trainer_name);
    }
};
%extend pkmn::pocket {
    void addItem(std::wstring input, unsigned int amount)
    {
        pkmn::pokemon_text item_name = pkmn::pokemon_text(input);
        self->add_item(item_name, amount);
    }

    void removeItem(std::wstring input, unsigned int amount)
    {
        pkmn::pokemon_text item_name = pkmn::pokemon_text(input);
        self->remove_item(item_name, amount);
    }

    unsigned int getItemAmount(std::wstring input)
    {
        pkmn::pokemon_text item_name = pkmn::pokemon_text(input);
        return self->get_item_amount(item_name);
    }
}
%extend pkmn::team_pokemon {
    std::wstring getSpeciesName() const
    {
        pkmn::pokemon_text species_name = self->get_species_name();
        return species_name.std_wstring();
    }

    std::wstring getNickname() const
    {
        pkmn::pokemon_text nickname = self->get_nickname();
        return nickname.std_wstring();
    }

    void setNickname(std::wstring input)
    {
        pkmn::pokemon_text nickname(input);
        self->set_nickname(nickname);
    }

    std::wstring getTrainerName() const
    {
        pkmn::pokemon_text trainer_name = self->get_trainer_name();
        return trainer_name.std_wstring();
    }

    std::wstring getTrainerGender() const
    {
        pkmn::pokemon_text trainer_gender = self->get_trainer_gender();
        return trainer_gender.std_wstring();
    }

    std::wstring getBall() const
    {
        pkmn::pokemon_text ball = self->get_ball();
        return ball.std_wstring();
    }

    void setTrainerName(std::wstring input)
    {
        pkmn::pokemon_text trainer_name(input);
        self->set_trainer_name(trainer_name);
    }

    void setTrainerGender(std::wstring input)
    {
        pkmn::pokemon_text trainer_gender(input);
        self->set_trainer_gender(trainer_gender);
    }

    void setBall(std::wstring input)
    {
        pkmn::pokemon_text ball(input);
        self->set_ball(ball);
    }

    std::wstring getStatus() const
    {
        pkmn::pokemon_text status = self->get_status();
        return status.std_wstring();
    }

    void setStatus(std::wstring input)
    {
        pkmn::pokemon_text status(input);
        self->set_status(status);
    }
};
%extend pkmn::trainer {
    static pkmn::trainer::sptr make(unsigned int game, std::wstring input, unsigned int gender)
    {
        pkmn::pokemon_text name(input);
        return pkmn::trainer::make(game, name, gender);
    }

    static pkmn::trainer::sptr make(std::string game, std::wstring input, std::string gender)
    {
        pkmn::pokemon_text name(input);
        return pkmn::trainer::make(game, name, gender);
    }
};

%include "CamelCase.i"
%include "stdint.i"
%include "std_pair.i"
%include "libpkmn.i"

%{
    #include "pkmn/config.hpp"

    #include "pkmn/paths.hpp"

    #include "pkmn/types/dict.hpp"
    #include "pkmn/types/pokemon_text.hpp"
    #include "pkmn/types/prng.hpp"

    #include "pkmn/nature.hpp"
    #include "pkmn/base_pokemon.hpp"
    #include "pkmn/item.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/bag.hpp"
    #include "pkmn/move.hpp"
    #include "pkmn/team_pokemon.hpp"
    #include "pkmn/trainer.hpp"
    #include "pkmn/game_save.hpp"

    #include "pkmn/lists.hpp"

%}

%include "pkmn/paths.hpp"

%include "pkmn/types/dict.hpp"
%include "pkmn/types/pokemon_text.hpp"
%include "pkmn/types/prng.hpp"

%include "pkmn/nature.hpp"
%include "pkmn/base_pokemon.hpp"
%include "pkmn/item.hpp"
%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"
%include "pkmn/move.hpp"
%include "pkmn/team_pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

%include "pkmn/lists.hpp"

//TODO: make C++ -> Java class name conversion function
%template(BagSPtr)         pkmn::shared_ptr<pkmn::bag>;
%template(BasePokemonSPtr) pkmn::shared_ptr<pkmn::base_pokemon>;
%template(GameSaveSPtr)    pkmn::shared_ptr<pkmn::game_save>;
%template(ItemSPtr)        pkmn::shared_ptr<pkmn::item>;
%template(MoveSPtr)        pkmn::shared_ptr<pkmn::move>;
%template(PocketSPtr)      pkmn::shared_ptr<pkmn::pocket>;
%template(PRNGSPtr)        pkmn::shared_ptr<pkmn::prng>;
%template(TeamPokemonSPtr) pkmn::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     pkmn::shared_ptr<pkmn::trainer>;

%template(BagSlot) std::pair<pkmn::item::sptr, unsigned int>;
%template(ItemList) std::vector<std::pair<pkmn::item::sptr, unsigned int> >;

%template(UIntPairVector) std::vector<std::pair<unsigned int, unsigned int> >;
%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(Moveset) std::vector<pkmn::move::sptr>;
%template(PocketVector) std::vector<pkmn::pocket::sptr>;
%template(PokemonTeam) std::vector<pkmn::team_pokemon::sptr>;

LIBPKMN_JAVA_DICT(StringIntDict, std::string, int, String, Integer, string_vec, int_vec)
LIBPKMN_JAVA_DICT(StringStringDict, std::string, std::string, String, String, string_vec, string_vec)
LIBPKMN_JAVA_DICT(StringUIntDict, std::string, unsigned int, String, Long, string_vec, uint_vec)
LIBPKMN_JAVA_DICT(PocketDict, std::string, pkmn::pocket::sptr, String, PocketSPtr, string_vec, PocketVector)

//Factory functions need to specifically be associated with newobject
%newobject pkmn::bag::make;
%newobject pkmn::base_pokemon::make;
%newobject pkmn::game_save::make;
%newobject pkmn::item::make;
%newobject pkmn::move::make;
%newobject pkmn::pocket::make;
%newobject pkmn::prng::get;
%newobject pkmn::team_pokemon::make;
%newobject pkmn::team_pokemon::get_base_pokemon;
%newobject pkmn::trainer::make;
