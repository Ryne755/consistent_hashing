#pragma once

#include <map>
#include <type_traits>

/*
 * consistent_hashing.h
 *
 *  Created on: May 24, 2019
 *      Author: rynzen <chuanrui123@126.com>
 *
 *  This file is part of a consistent hashing implemenation.
 *
 *  consistent_hashing.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  consistent_hashing.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with consistent_hashing.  If not, see <http://www.gnu.org/licenses/>.
 */

 
// simple implementation of consistent hashing algorithm for distribute system
// Example:
//    struct Node{}
//    struct Hash
//	  {
//		UInt32 operator()(const Node & node)
//		{
//		}
//    }
//    Hash hash_obj;
//    ConsistentHashMap<UInt32, Node> h;
//    h.insert(Node(), hash_obj);
//    h.find(hash_obj(node));

template <typename Key, typename Node,
	class Compare = std::less<Key>,
	class Allocator = std::allocator<std::pair<const Key, Node>>>
	class ConsistentHashMap : public std::map<Key, Node, Compare, Allocator>
{
public:
	using const_iterator = typename std::map<Key, Node, Compare, Allocator>::const_iterator;
	using iterator = typename std::map<Key, Node, Compare, Allocator>::iterator;
	using key_type = typename std::map<Key, Node, Compare, Allocator>::key_type;
	using mapped_type = typename std::map<Key, Node, Compare, Allocator>::mapped_type;
	explicit ConsistentHashMap(const Compare& comp = Compare(),
		const Allocator& alloc = Allocator()) : std::map<Key, Node, Compare, Allocator>(comp, alloc) {}
	~ConsistentHashMap() = default;
	const_iterator find(const key_type& key) const
	{
		const_iterator it = this->lower_bound(key);
		if (false == this->empty() && it == this->end())
			it = this->begin();
		return it;
	}

	template <typename HashMethod>
	std::pair<iterator, bool> insert(const Node & node, HashMethod & method)
	{
		static_assert(std::is_same_v<key_type, std::decay_t<decltype(method(node))>>, "error hash method");
		key_type key = method(node);
		return std::map<Key, Node, Compare, Allocator>::insert({ key, node });
	}
};
