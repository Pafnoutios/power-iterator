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
    using difference_type = void; ///<  I don't want to try to compute distances between these if I don't have to.
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
    {
      if (end)
      {
        m_members.reserve(std::distance(m_begin, m_end) + 1);
        for (source_iterator i = m_begin; i != m_end; ++i)
        {
          m_members.push_back(i);
        }
        m_members.push_back(m_end);
      }
      // else, the first element will be an empty set.
    }

    bool operator==(const_iterator const& rhs) const
    {
      return (m_begin == rhs.m_begin)
        && (m_end == rhs.m_end)
        && (m_members == rhs.m_members);
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
      calculate_value();
      return m_value;
    }

  private:

    void calculate_value()
    {
      m_value.clear();
      for (auto& x : m_members)
        m_value.insert(*x);
    }

    void increment()
    {
      // Increment if not at end
      if (m_members.size() <= std::distance(m_begin, m_end))
      {

      }
    }

    source_iterator m_begin;
    source_iterator m_end;
    std::vector<source_iterator, Allocator> m_members;
    value_type m_value;
  };

  using iterator = const_iterator;

  powerset(key_type source)
    : powerset(source.begin(), source.end())
  {

  }

  powerset(source_iterator source_begin, source_iterator source_end)
    : m_begin(source_begin)
    , m_end(source_end)
  {

  }

  bool operator==(const powerset &rhs) const
  {
    return (size() == rhs.size())
      && std::equal(m_begin, m_end, rhs.m_begin);
  }

  
  size_type size() const
  {
    renurn m_size;
  }

private:

  size_type evaluate_size() const
  {
    auto n = std::distance(m_begin, m_end);
    return static_cast<size_type>(2) << n;
  }

  source_iterator m_begin;
  source_iterator m_end;
  MemoizedMember<size_type, powerset, &powerset::evaluate_size> m_size{ *this };
};
