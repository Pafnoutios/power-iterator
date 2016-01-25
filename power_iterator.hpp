/**
*	\author    John Szwast
*	\year      2014-2016
*	\copyright MIT
*/


#pragma once


#include <memory>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include "MemoizedMember.hpp"


using std::rel_ops::operator!=;

template<typename Key,
class Compare = std::less<Key>,
class Allocator = std::allocator<Key >>
class powerset
{
public:
  using key_type = std::set<Key, Compare, Allocator>;
  using value_type = key_type;
  using allocator_type = Allocator;
  using size_type = typename value_type::size_type;
  using difference_type = typename value_type::difference_type;


  using source_iterator = typename value_type::const_iterator;

  class const_iterator
  {

  };


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
