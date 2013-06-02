/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_DICT_IPP
#define INCLUDED_DICT_IPP

#include <stdexcept>
#include <string>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    template <typename Key, typename Val>
    dict<Key, Val>::dict(void) {}

    template <typename Key, typename Val> template <typename InputIterator>
    dict<Key, Val>::dict(InputIterator first, InputIterator last):
        _map(first, last) {}

    template <typename Key, typename Val>
    std::size_t dict<Key, Val>::size(void) const
    {
        return _map.size();
    }

    template <typename Key, typename Val>
    std::vector<Key> dict<Key, Val>::keys(void) const
    {
        std::vector<Key> keys;
        FOREACH(const dict_pair &p, _map)
        {
            keys.push_back(p.first);
        }
        return keys;
    }

    template <typename Key, typename Val>
    std::vector<Val> dict<Key, Val>::vals(void) const
    {
        std::vector<Val> vals;
        FOREACH(const dict_pair &p, _map)
        {
            vals.push_back(p.second);
        }
        return vals;
    }

    template <typename Key, typename Val>
    bool dict<Key, Val>::has_key(const Key &key) const
    {
        FOREACH(const dict_pair &p, _map)
        {
            if (p.first == key) return true;
        }
        return false;
    }

    template <typename Key, typename Val>
    const Val &dict<Key, Val>::get(const Key &key, const Val &other) const
    {
        FOREACH(const dict_pair &p, _map)
        {
            if (p.first == key) return p.second;
        }
        return other;
    }

    template <typename Key, typename Val>
    const Val &dict<Key, Val>::get(const Key &key) const
    {
        FOREACH(const dict_pair &p, _map)
        {
            if (p.first == key) return p.second;
        }
        std::string err_msg = "Key '" + to_string(key) + "' not found.";
        throw std::runtime_error(err_msg.c_str());
    }

    template <typename Key, typename Val>
    void dict<Key, Val>::set(const Key &key, const Val &val)
    {
        (*this)[key] = val;
    }

    template <typename Key, typename Val>
    const Val &dict<Key, Val>::operator[](const Key &key) const
    {
        FOREACH(const dict_pair &p, _map)
        {
            if (p.first == key) return p.second;
        }
        std::string err_msg = "Key '" + to_string(key) + "' not found.";
        throw std::runtime_error(err_msg.c_str());
    }

    template <typename Key, typename Val>
    Val &dict<Key, Val>::operator[](const Key &key)
    {
        FOREACH(dict_pair &p, _map)
        {
            if (p.first == key) return p.second;
        }
        _map.push_back(std::make_pair(key, Val()));
        return _map.back().second;
    }

    template <typename Key, typename Val>
    Val dict<Key, Val>::pop(const Key &key)
    {
        typename std::list<dict_pair>::iterator it;
        for (it = _map.begin(); it != _map.end(); it++)
        {
            if (it->first == key)
            {
                Val val = it->second;
                _map.erase(it);
                return val;
            }
        }
        std::string err_msg = "Key '" + to_string(key) + "' not found.";
        throw std::runtime_error(err_msg.c_str());
    }

}

#endif /* INCLUDED_DICT_IPP */
