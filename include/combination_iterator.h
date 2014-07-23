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
#include <algorithm>


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
			using source_iterator = typename combinations<Key, Compare, Allocator>::source_iterator;

			const_iterator(source_iterator source_begin, source_iterator source_end, size_type r, bool end = false)
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

			bool operator==(const const_iterator &rhs) const
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

			const std::set<Key, Compare, Allocator>& operator*()
			{
				calculate_value();
				return m_value;
			}

		private:

			void calculate_value()
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
						&& (*--gopher != --reference))
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
			std::set<Key, Compare, Allocator> m_value;	// The value returned by dereferencing.
			bool m_at_end;	// If m_r == 0, then m_members is always empty and there is no distiction
				              // between begin and end.  This flag will indicate when the end has been reached.
		};

		combinations(source_iterator source_begin, source_iterator source_end, size_type r)
		: m_begin(source_begin)
		, m_end(source_end)
		, m_r(r)
		{

		}

		bool operator==(const combinations &rhs) const
		{
			return (m_r == rhs.m_r)
				&& (((m_end == rhs.m_end) && (m_begin == rhs.m_begin))
					|| ((size() == rhs.size()) && std::equal(m_begin, m_end, rhs.m_begin)));
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

