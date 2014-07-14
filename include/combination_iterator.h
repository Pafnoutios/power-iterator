/**
 *	@file      combination_iterator.h
 *	@author    John Szwast
 *	@year      2014
 *	@copyright none
 */


#pragma once


#include <memory>
#include <set>
#include <vector>
#include <utility>


namespace szwast
{
	using std::rel_ops::operator!=;


	template<typename Key,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<Key>>
	class combinations
	{
	public:
		using value_type      = std::set<Key, Compare, Allocator>;
		using allocator_type  = Allocator;
		using size_type       = std::size_t;

		using source_iterator = typename value_type::const_iterator;

		class const_iterator
		{
		public:
			using source_iterator = combinations<Key, Compare, Allocator>::source_iterator;

			const_iterator(source_iterator source_begin, source_iterator source_end, size_type size, bool end = false)
			: m_begin(source_begin)
			, m_end(source_end)
			, m_size(size)
			, m_members(m_size)
			{
				source_iterator gopher = m_begin;

				if (end)
				{
					// Start gopher at m_size before end, to put the last m_member at end.
					gopher = m_end;
					for (size_type i = 1; (i < m_size) && (gopher != m_begin); i++)
					{
						gopher--;
					}
				}

				for (auto &i : m_members)
				{
					i = gopher;
					if (gopher != m_end)
					{
						gopher++;
					}
				}
			}

			bool operator==(const const_iterator &rhs) const
			{
				return (m_begin == rhs.m_begin)
					&& (m_end == rhs.m_end)
					&& (m_members == rhs.m_members);
			}

		private:

			source_iterator m_begin;
			source_iterator m_end;
			size_type m_size;	// I might not need this, because it is embedded in m_members.
			std::vector<source_iterator, Allocator> m_members;
		};

		combinations(source_iterator source_begin, source_iterator source_end, size_type size)
		: m_begin(source_begin)
		, m_end(source_end)
		, m_size(size)
		{

		}

		const_iterator begin() const
		{
			return const_iterator(m_begin, m_end, m_size, false);
		}

		const_iterator end() const
		{
			return const_iterator(m_begin, m_end, m_size, true);
		}

	private:
		source_iterator m_begin;
		source_iterator m_end;
		size_type m_size;
	};
}

