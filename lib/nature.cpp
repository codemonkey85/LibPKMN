/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>

#include <pkmn/enums.hpp>
#include <pkmn/nature.hpp>
#include <pkmn/database/queries.hpp>

namespace pkmn
{
    nature::nature(unsigned int id)
    {
        _id = id;
        _name = database::get_nature_name(id);

        _populate_dict();
    }

    nature::nature(std::string name)
    {
        _name = name;
        _id = database::get_nature_id(name);

        _populate_dict();
    }

    std::string nature::get_name() {return _name;}

    const double& nature::operator[](const std::string &key) const
    {
        return _modifiers[key];
    }    

    unsigned int nature::get_nature_id() {return _id;}

    void nature::_populate_dict()
    {
        _modifiers["Attack"] = database::get_nature_stat_effect(_id, Stats::ATTACK);
        _modifiers["Defense"] = database::get_nature_stat_effect(_id, Stats::DEFENSE);
        _modifiers["Special Attack"] = database::get_nature_stat_effect(_id, Stats::SPECIAL_ATTACK);
        _modifiers["Special Defense"] = database::get_nature_stat_effect(_id, Stats::SPECIAL_DEFENSE);
        _modifiers["Speed"] = database::get_nature_stat_effect(_id, Stats::SPEED);
    }

} /* namespace pkmn */
