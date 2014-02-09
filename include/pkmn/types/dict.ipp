/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_DICT_IPP
#define INCLUDED_PKMN_TYPES_DICT_IPP

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <pkmn/config.hpp>

namespace pkmn
{
    template <typename Key, typename Val>
    dict<Key, Val>::dict(void) {}

    template <typename Key, typename Val> template <typename InputIterator>
    dict<Key, Val>::dict(InputIterator first, InputIterator last):
        _map(first, last) {}

    template<typename Key, typename Val>
    bool dict<Key, Val>::empty() const {return _map.empty();}

    template<typename Key, typename Val>
    size_t dict<Key, Val>::size() const {return _map.size();}

    template <typename Key, typename Val>
    const Val& dict<Key, Val>::operator[](const Key& key) const
    {
        FOREACH(const dict_pair& p, _map.begin(), _map.end())
        {
            if(p.first == key) return p.second;
        }
        std::string err_msg = "Key '" + to_string(key) + "' not found.";
        throw std::runtime_error(err_msg.c_str());
    }

    template <typename Key, typename Val>
    Val& dict<Key, Val>::operator[](const Key& key)
    {
        FOREACH(dict_pair& p, _map.begin(), _map.end())
        {
            if(p.first == key) return p.second;
        }
        _map.push_back(std::make_pair(key, Val()));
        return _map.back().second;
    }

    template <typename Key, typename Val>
    const Val& dict<Key, Val>::at(const Key& key) const
    {
        FOREACH(const dict_pair& p, _map.begin(), _map.end())
        {
            if(p.first == key) return p.second;
        }
        std::string err_msg = "Key '" + to_string(key) + "' not found.";
        throw std::runtime_error(err_msg.c_str());
    }

    template <typename Key, typename Val>
    const Val& dict<Key, Val>::at(const Key& key, const Val& other) const
    {
        FOREACH(const dict_pair& p, _map.begin(), _map.end())
        {
            if(p.first == key) return p.second;
        }
        return other;
    }

    template <typename Key, typename Val>
    void dict<Key, Val>::insert(const Key& key, const Val& val)
    {
        FOREACH(dict_pair& p, _map.begin(), _map.end())
        {
            if(p.first == key)
            {
                std::string err_msg = "Key '" + to_string(key) + "' already exists.";
                throw std::runtime_error(err_msg.c_str());
            }
        }
        _map.push_back(std::make_pair(key, val));
    }

    template <typename Key, typename Val>
    void dict<Key, Val>::erase(const Key& key)
    {
        typename std::list<std::pair<Key,Val> >::iterator it = _map.begin();
        while(it != _map.end())
        {
            if((*it).first == key)
            {
                _map.erase(it);
                return;
            }
        }
        std::string err_msg = "Key '" + to_string(key) + "' not found.";
        throw std::runtime_error(err_msg.c_str());
    }

    template <typename Key, typename Val>
    void dict<Key, Val>::swap(Key key1, Key key2)
    {
        std::string err_msg;
        if(not has_key(key1))
        {
            err_msg = "Key '" + to_string(key1) + "' not found.";
            throw std::runtime_error(err_msg.c_str());
        }
        if(not has_key(key1))
        {
            err_msg = "Key '" + to_string(key2) + "' not found.";
            throw std::runtime_error(err_msg.c_str());
        }
        Val temp = at(key1);
        (*this)[key1] = (*this)[key2];//_map[key1] = _map[key2];
        (*this)[key2] = temp;//_map[key2] = temp;
    }

    template <typename Key, typename Val>
    void dict<Key, Val>::clear() {_map.clear();}

    template <typename Key, typename Val>
    bool dict<Key, Val>::has_key(const Key &key) const
    {
        FOREACH(const dict_pair &p, _map.begin(), _map.end())
        {
            if (p.first == key) return true;
        }
        return false;
    }

    template <typename Key, typename Val>
    std::vector<Key> dict<Key, Val>::keys(void) const
    {
        std::vector<Key> keys;
        FOREACH(const dict_pair &p, _map.begin(), _map.end())
        {
            keys.push_back(p.first);
        }
        return keys;
    }

    template <typename Key, typename Val>
    std::vector<Val> dict<Key, Val>::vals(void) const
    {
        std::vector<Val> vals;
        FOREACH(const dict_pair &p, _map.begin(), _map.end())
        {
            vals.push_back(p.second);
        }
        return vals;
    }
}

#endif /* INCLUDED_PKMN_TYPES_DICT_IPP */
