/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_DICT_HPP
#define INCLUDED_PKMN_TYPES_DICT_HPP

#include <list>
#include <vector>

#include <pkmn/config.hpp>

namespace pkmn
{
    template <typename Key, typename Val> class dict
    {
        public:
        
            dict(void);

            template <typename InputIterator>
            dict(InputIterator first, InputIterator last);

            bool empty() const;
            size_t size() const;
            const Val& operator[](const Key& key) const;
            Val& operator[](const Key& key);
            const Val& at(const Key& key) const;
            const Val& at(const Key& key, const Val& other) const;
            void insert(const Key& key, const Val& val);
            void erase(const Key& key);
            void swap(Key key1, Key key2);
            void clear();

            bool has_key(const Key& key) const;
            std::vector<Key> keys() const;
            std::vector<Val> vals() const;

        private:
        
            typedef std::pair<Key, Val> dict_pair;
            std::list<dict_pair> _map;
        };

}

#include <pkmn/types/dict.ipp>

#endif /* INCLUDED_PKMN_TYPES_DICT_HPP */
