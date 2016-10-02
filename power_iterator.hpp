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

template<typename Key,
class Compare = std::less<Key>,
class Allocator = std::allocator<Key>>
class powerset
{
public:
  using key_type = std::set<Key, Compare, Allocator>;
  using value_type = key_type;
  using allocator_type = Allocator;
  using size_type = typename value_type::size_type;
  using difference_type = typename value_type::difference_type;

  using reference = value_type&;
  using const_reference = value_type const&;
  using pointer = value_type*;
  using const_pointer = value_type const*;

  using source_iterator = typename value_type::const_iterator;

  class const_iterator
  {
  public:
    using powers_type = typename powerset<Key, Compare, Allocator>;
    using source_iterator = typename powers_type::source_iterator;
    using mutable_value_type = typename powers_type::key_type;

    /// Type_traits aliases
    using difference_type = typename mutable_value_type::const_iterator::difference_type;
    using value_type = mutable_value_type const;
    using pointer = value_type const*;
    using reference = value_type const&;
    using iterator_category = std::forward_iterator_tag;

    const_iterator(
      source_iterator const source_begin,
      source_iterator const source_end,
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

    using combinations_type = combinations<Key, Compare, Allocator>;


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

    source_iterator m_begin;
    source_iterator m_end;
    size_type m_n;
    size_type m_r;
    combinations_type m_combinations;
    typename combinations_type::const_iterator m_iter;
  };

  using iterator = const_iterator;

  powerset(key_type const& source)
    : powerset(source.cbegin(), source.cend())
  {

  }

  powerset(source_iterator source_begin, source_iterator source_end)
    : m_begin(source_begin)
    , m_end(source_end)
  {

  }

  powerset(powerset const& rhs)
    : m_begin(rhs.m_begin)
    , m_end(rhs.m_end)
    , m_size(*this, rhs.m_size)
  {

  }


  powerset(powerset&& rhs)
    : m_begin(std::move(rhs.m_begin))
    , m_end(std::move(rhs.m_end))
    , m_size(*this, std::move(rhs.m_size))
  {}


  powerset& operator=(powerset const&) = default;
  powerset& operator=(powerset&&) = default;


  bool operator==(const powerset &rhs) const
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

  size_type evaluate_size() const
  {
    auto n = std::distance(m_begin, m_end);
    return static_cast<size_type>(1) << n;
  }

  source_iterator m_begin;
  source_iterator m_end;
  MemoizedMember<size_type, powerset, &powerset::evaluate_size> m_size{ *this };
};
