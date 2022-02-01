# Combination and Power Iterators

## Combination Iterators

### Combinations Virtual Container

A `combinations` virtual container is used to represent the set of combinations.
It is a lazy container and does not calculate anything itself.
It provides a means to acquire `begin` and `end` iterators for algorithms
and allows the `combinations` to be iterated over in a range-for loop. 

A combinations object may be created from a `std::set` container
(providing `begin()` and `end()` methods)
or a pair of STL-compatible iterators to the same container.
The combinations size (the `r` in `nCr`) must be specified at the time of construction.
This is taken as a constructor parameter to allow combination objects to be reused
and to allow the `r` to be determined at run-time.

    combinations(std::set<T> source, std::size_t r);
    combinations(std::set<T>::const_iterator begin,
                 std::set<T>::const_iterator end,
                 std::size_t r);

The combinations object provides `begin()` and `end()` member functions for full-range iteration.

    const_iterator begin() const;
    const_iterator end() const;

### Internal Iterators

A combinations iterator has a number of item iterators, `r`,
equal in number to the size of combinations being iterated over.
The combination iterator `begin()` is represented
by item iterators of [`begin()`, `begin()+r`).
The combination iterator `end()` is represented
by item iterators of (`end()-r`, `end()`].
Incrementation is implemented by incrementing the end-most iterator
that has room to move end-ward, and resetting all later iterators
in a contiguous sequence after it.
(See Figure 1 for an example sequence of combination iterators
over combinations of size 3 of a base set of size 5.)

![Combinations (5,3) Example](Combinations(5,3)Example.svg)

Figure 1: Iterator sequence for combinations of size 3 over a set of size 5.

In this way, combination iterators will produce combinations
in lexicographical order with respect to the order in the underlying container.



## Power Set Iterator

The power set iterator is implemented with combination iterators.
One combination iterator is used at a time,
in increasing order of combination size.
Each combination iterator is incremented through its entire range
before moving on to the iterator of next higher combination size.

This is easy to implement, but fails to produce sets in lexicographical order.