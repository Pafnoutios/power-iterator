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

#include "combination_iterator.hpp"
#include "MemoizedMember.hpp"


using std::rel_ops::operator!=;

template<
	class Iter,
	class Allocator = std::allocator<typename Iter::value_type>
>
class powerset
{
public:

	using key_type = std::vector<typename Iter::value_type, Allocator>;
	using value_type = key_type;
	using allocator_type = Allocator;
	using size_type = typename value_type::size_type;
	using difference_type = typename value_type::difference_type;

	using reference = value_type&;
	using const_reference = value_type const&;
	using pointer = value_type*;
	using const_pointer = value_type const*;


	class const_iterator
	{
	public:

		using powers_type = powerset;
		using mutable_value_type = typename powers_type::key_type;

		/// Type_traits aliases
		using difference_type = typename mutable_value_type::const_iterator::difference_type;
		using value_type = mutable_value_type const;
		using pointer = value_type const*;
		using reference = value_type const&;
		using iterator_category = std::forward_iterator_tag;

		const_iterator(
			Iter const source_begin,
			Iter const source_end,
			bool const end = false)
			: m_begin(source_begin)
			, m_end(source_end)
			, m_n(std::distance(m_begin, m_end))
			, m_r(end ? m_n + 1 : 0)
			, m_combinations(m_begin, m_end, m_r)
			, m_iter(m_combinations.begin())
		{
		}

		bool operator==(const_iterator const& rhs) const
		{
			return (m_begin == rhs.m_begin)
				&& (m_end == rhs.m_end)
				&& (m_r == rhs.m_r)
				&& (m_iter == rhs.m_iter);
		}

		const_iterator& operator++()
		{
			increment();
			return *this;
		}

		const_iterator operator++(int)
		{
			const_iterator temp{ *this };
			increment();
			return temp;
		}

		reference operator*() const
		{
			return *m_iter;
		}

	private:

		using combinations_type = combinations<Iter, Allocator>;


		void increment()
		{
			// Increment if not at end
			if (!at_end())
			{
				++m_iter;
				if (m_iter == m_combinations.end())
				{
					++m_r;
					m_combinations = combinations_type(m_begin, m_end, m_r);
					m_iter = m_combinations.begin();
				}
			}
		}

		bool at_end()
		{
			return m_r > m_n;
		}

		Iter m_begin;
		Iter m_end;
		size_type m_n;
		size_type m_r;
		combinations_type m_combinations;
		typename combinations_type::const_iterator m_iter;
	};

	using iterator = const_iterator;


	powerset(Iter source_begin, Iter source_end)
		: m_begin(source_begin)
		, m_end(source_end)
	{}


	template<class Container>
	powerset(Container const& source)
		: powerset(source.cbegin(), source.cend())
	{}


	powerset(powerset const& rhs)
		: m_begin(rhs.m_begin)
		, m_end(rhs.m_end)
		, m_size(*this, rhs.m_size)
	{}


	powerset(powerset&& rhs)
		: m_begin(std::move(rhs.m_begin))
		, m_end(std::move(rhs.m_end))
		, m_size(*this, std::move(rhs.m_size))
	{}


	powerset& operator=(powerset const&) = default;
	powerset& operator=(powerset&&) = default;


	template<class Iter2, class Alloc>
	bool operator==(const powerset<Iter2, Alloc> &rhs) const
	{
		return (size() == rhs.size())
			&& std::equal(m_begin, m_end, rhs.m_begin);
	}


	const_iterator begin() const
	{
		return const_iterator(m_begin, m_end, false);
	}

	const_iterator cbegin() const
	{
		return const_iterator(m_begin, m_end, false);
	}

	const_iterator end() const
	{
		return const_iterator(m_begin, m_end, true);
	}

	const_iterator cend() const
	{
		return const_iterator(m_begin, m_end, true);
	}
  
	size_type size() const
	{
		return m_size;
	}

private:

	template<class Iter2, class Alloc>
	friend class powerset;


	size_type evaluate_size() const
	{
		auto n = std::distance(m_begin, m_end);
		return static_cast<size_type>(1) << n;
	}

	Iter m_begin;
	Iter m_end;
	MemoizedMember<size_type, powerset, &powerset::evaluate_size> m_size{ *this };
};


template<typename Container>
powerset<typename Container::const_iterator> make_powerset(Container const& source)
{
	return powerset<typename Container::const_iterator>{source};
}


template<typename Iter>
powerset<Iter> make_powerset(Iter begin, Iter end)
{
	return powerset<Iter>{begin, end};
}

