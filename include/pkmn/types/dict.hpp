/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_TYPES_DICT_HPP
#define INCLUDED_PKMNSIM_TYPES_DICT_HPP

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

            std::size_t size(void) const;

            std::vector<Key> keys(void) const;

            std::vector<Val> vals(void) const;

            bool has_key(const Key &key) const;

            const Val &get(const Key &key, const Val &other) const;

            const Val &get(const Key &key) const;

            void set(const Key &key, const Val &val);

            const Val &operator[](const Key &key) const;

            Val &operator[](const Key &key);

            Val pop(const Key &key);

        private:
        
            typedef std::pair<Key, Val> dict_pair;
            std::list<dict_pair> _map;
        };

}

#include <pkmn/types/dict.ipp>

#endif /* INCLUDED_PKMNSIM_TYPES_DICT_HPP */
