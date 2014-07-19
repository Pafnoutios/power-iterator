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

	template<typename Key, class Class>
	class MemoizedMember
	{
	public:
		MemoizedMember(Class& instance, Key (Class::*evaluate)() const)
		: m_instance(instance)
		, m_evaluate(evaluate)
		{
		}

		MemoizedMember& operator=(const MemoizedMember& r)
		{
			m_valid = r.m_valid;
			m_value = r.m_value;
			m_evaluate = r.m_evaluate;
			return *this;
		}

		MemoizedMember& operator=(const MemoizedMember&& r)
		{
			m_valid = r.m_valid;
			m_value = r.m_value;
			m_evaluate = r.m_evaluate;
			return *this;
		}

		operator Key() const
		{
			if (!m_valid)
			{
				m_value = (m_instance.*m_evaluate)();
				m_valid = true;
			}
			return m_value;
		}

	private:
		mutable bool m_valid{false};
		Class& m_instance;
		mutable Key m_value{};
		Key (Class::*m_evaluate)() const;
	};


	template<typename Key,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<Key>>
	class combinations
	{
	public:
		using value_type      = std::set<Key, Compare, Allocator>;
		using allocator_type  = Allocator;
		using size_type       = typename std::set<Key, Compare, Allocator>::size_type;

		using source_iterator = typename value_type::const_iterator;

		class const_iterator
		{
		public:
			using source_iterator = combinations<Key, Compare, Allocator>::source_iterator;

			const_iterator(source_iterator source_begin, source_iterator source_end, size_type r, bool end = false)
			: m_begin(source_begin)
			, m_end(source_end)
			, m_r(r)
			, m_members(m_r)
			{
				source_iterator gopher = m_begin;

				if (end)
				{
					// Start gopher at m_size before end, to put the last m_member at end.
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
			size_type m_r;	// r as in nCr.  I might not need this, because it is embedded in m_members.
			std::vector<source_iterator, Allocator> m_members;
		};

		combinations(source_iterator source_begin, source_iterator source_end, size_type r)
		: m_begin(source_begin)
		, m_end(source_end)
		, m_r(r)
		{

		}

		bool operator==(const combinations &rhs) const
		{
			return (m_begin == rhs.m_begin)
				&& (m_end == rhs.m_end)
				&& (m_size == rhs.m_size);
		}

		const_iterator begin() const
		{
			return const_iterator(m_begin, m_end, m_r, false);
		}

		const_iterator end() const
		{
			return const_iterator(m_begin, m_end, m_r, true);
		}

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
		};
		source_iterator m_begin;
		source_iterator m_end;
		size_type m_r;	// 'r' as in nCr.
		MemoizedMember<size_type, combinations> m_size{*this, &combinations::evaluate_size};

	};
}

