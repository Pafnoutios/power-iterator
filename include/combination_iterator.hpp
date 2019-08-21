/**
*	\author    John Szwast
*	\year      2014-2016
*	\copyright MIT
*/


#pragma once


#include <algorithm>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "MemoizedMember.hpp"


using std::rel_ops::operator!=;

template<typename Key,
	class Compare = std::less<Key>,
	class Allocator = std::allocator<Key>>
class combinations
{
public:
  using key_type        = std::set<Key, Compare, Allocator>;
  using value_type      = key_type;
	using allocator_type  = Allocator;
	using size_type       = typename value_type::size_type;
  using difference_type = typename value_type::difference_type;

  using reference = value_type&;
  using const_reference = value_type const&;
  using pointer = value_type*;
  using const_pointer = value_type const*;

	using source_iterator = typename value_type::const_iterator;

	class const_iterator
	{
	public:
    using combinations_type = typename combinations<Key, Compare, Allocator>;
		using source_iterator = typename combinations_type::source_iterator;
    using mutable_value_type = typename combinations_type::key_type;

    /// Type_traits aliases
    using difference_type = typename mutable_value_type::const_iterator::difference_type;
    using value_type = mutable_value_type const;
    using pointer = value_type const*;
    using reference = value_type const&;
    using iterator_category = std::forward_iterator_tag;

		const_iterator(
      source_iterator const source_begin,
      source_iterator const source_end,
      size_type const r,
      bool const end = false
		)
		: m_begin(source_begin)
		, m_end(source_end)
		, m_r(r)
		, m_members(m_r)
		, m_at_end(end)
		{
			source_iterator gopher = m_begin;

			if (m_at_end)
			{
				// Start gopher at m_size-1 before end, to put the last m_member at end.
				gopher = m_end;
				for (size_type i = 1; (i < m_r) && (gopher != m_begin); i++)
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

			// If we were constructing a begin, but r > end - begin, then we're also at end.
			m_at_end = m_at_end || (!m_members.empty() && (m_members.back() == m_end));
		}

		bool operator==(const_iterator const& rhs) const
		{
			return (m_begin == rhs.m_begin)
				&& (m_end == rhs.m_end)
				&& (m_members == rhs.m_members)
				&& (m_at_end == rhs.m_at_end);
		}

		const_iterator& operator++()
		{
			increment();
			return *this;
		}

		const_iterator operator++(int)
		{
			const_iterator temp{*this};
			increment();
			return temp;
		}

		reference operator*() const
		{
			calculate_value();
			return m_value;
		}

	private:

		void calculate_value() const
		{
			m_value.clear();
			for (auto &x : m_members)
				m_value.insert(*x);
		}

		void increment()
		{
			// Increment if not at end
			if (!m_members.empty() && !m_at_end)
			{
				auto gopher = m_members.end();
				auto reference = m_end;

				// Find the first member from the end that has room to move forward
				while ((gopher != m_members.begin())
					&& (*--gopher == --reference))
				{}

				// Move it forward and put the rest right after
				for (reference = *gopher; gopher != m_members.end(); gopher++)
					(*gopher) = ++reference;
			}

			m_at_end = m_members.empty() || (m_members.back() == m_end);
		}

		source_iterator m_begin;
		source_iterator m_end;
		size_type m_r;	// r as in nCr.  I might not need this, because it is embedded in m_members.
		std::vector<source_iterator, Allocator> m_members;
		mutable mutable_value_type m_value;	// The value returned by dereferencing.
		bool m_at_end;	// If m_r == 0, then m_members is always empty and there is no distinction
				            // between begin and end.  This flag will indicate when the end has been reached.
	};

  using iterator = const_iterator;

  combinations(key_type const& source, size_type r)
    : combinations(source.begin(), source.end(), r)
  {

  }

	combinations(source_iterator source_begin, source_iterator source_end, size_type r)
	: m_begin(source_begin)
	, m_end(source_end)
	, m_r(r)
	{

	}

  combinations(combinations const& rhs)
    : m_begin(rhs.m_begin)
    , m_end(rhs.m_end)
    , m_r(rhs.m_r)
    , m_size(*this, rhs.m_size)
  {

  }


  combinations& operator=(combinations const&) = default;
  combinations& operator=(combinations&&) = default;


  combinations(combinations&& rhs)
    : m_begin(std::move(rhs.m_begin))
    , m_end(std::move(rhs.m_end))
    , m_r(std::move(rhs.m_r))
    , m_size(*this, std::move(rhs.m_size))
  {}

	/**
	 *	@remarks
	 *		I wanted to let a quick (m_begin == rhs.m_begin && m_end == rhs.m_end) short-circuit
	 *	the possibly laborious std::equal, but MSVC gives a run-time assert whenever comparing
	 *	iterators from different containers.  I'm trying not to get too upset about it, because
	 *	I can understand the sentiment.  None-the-less it has proven rather irritating.
	 */
	bool operator==(const combinations &rhs) const
	{
		return (m_r == rhs.m_r)
			&& (size() == rhs.size())
      && std::equal(m_begin, m_end, rhs.m_begin);
	}

	const_iterator begin() const
	{
		return const_iterator(m_begin, m_end, m_r, false);
	}

  const_iterator cbegin() const
  {
    return const_iterator(m_begin, m_end, m_r, false);
  }

  const_iterator end() const
	{
		return const_iterator(m_begin, m_end, m_r, true);
	}

  const_iterator cend() const
  {
    return const_iterator(m_begin, m_end, m_r, true);
  }

  /**
	 * The size() method will return the number of combination sets in the combinations class,
	 * but it not a simple getter.  Evaluating the number of combinations requires knowledge of
	 * the number of elements in the base container.  Finding the number of elements in the base
	 * container may not be a constant-time operation.  If the source iterators are only
	 * forward iterators, not random-access, then finding the size of the base container will
	 * cost on the order of the size of the base container.  Once calculated, the size will be
	 * cached for successive calls.
	 *
	 * \see evaluate_size()
	 */
	size_type size() const
	{
		return m_size;
	}

private:

	size_type evaluate_size() const
	{
		auto n = std::distance(m_begin, m_end);
		size_type r_max = std::min(m_r, n - m_r);
		size_type size{1};
		for (size_type r = 0; r++ < r_max; n--)
			(size *= n) /= r;
		return size;
	}

	source_iterator m_begin;
	source_iterator m_end;
	size_type m_r;	// 'r' as in nCr.
	MemoizedMember<size_type, combinations, &combinations::evaluate_size> m_size{*this};

};


template<typename T>
combinations<T> make_combinations(std::set<T> const& source, typename combinations<T>::size_type r)
{
  return combinations<T>{source, r};
}


template<typename Iter, typename T = typename Iter::value_type>
combinations<T> make_combinations(Iter begin, Iter end, typename combinations<T>::size_type r)
{
  return combinations<T>{begin, end, r};
}

