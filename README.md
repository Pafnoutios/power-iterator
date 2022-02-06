# power-iterator

This library contains facilities for iterating over the power set of a collection
as well as over combinations of various size of a collection.

## Power Set

The power set of a collection is the set of all subsets.

The `powerset` class is a virtual container
representing the power set of a particular source collection.
A `powerset` class does not actually store all of the subsets,
but merely enough information to generate them.
The iterator will generate them on demand.

### Usage

Instantiate an instance of a `powerset<T>` with `begin` and `end` iterators from a source `std::set<T>`.

    std::set<T> source_set;
    ...
    powerset<T> subsets(source_set.begin(), source_set.end());

From this point, the `powerset<T>::const_iterator` will be able to iterate
over all the subsets of `source_set`.

    for(auto const& subset : subsets)
    {
        // Do stuff with subset
    }

### Behavior

The subsets are generated in increasing order of size.
The empty set first,
then subsets of size 1,
then of size 2,
and so on.
Each subsequence of subsets of the same size
are generated in lexicographical order.
But the entire sequence is not in lexicographic order.

## Combinations

The combinations of a collection is the set of all subsets of a particular size.

The `combinations` class is a virtual container
representing the combinations of a particular size of a source collection.
A `combinations` object does not actually store all of the combinations,
but merely enough information to generate them.
The iterator will generate them on demand.

### Usage

Instantiate an instance of a `combinations<T>` with `begin` and `end` iterators from a source `std::set<T>`
and the size of subsets desired.

    std::set<T> source_set;
    ...
    combinations<T> subsets_of_3(source_set.begin(), source_set.end(), 3);

From this point, the `combinations<T>::const_iterator` will be able to iterate
over all subsets of `source_set` of size 3.

    for(auto const& subset : subsets_of_3)
    {
        // Do stuff with subset
    }

### Behavior

The combinations are generated in lexicographical order.

## Known limitations

The `combinations` class currently only works with `std::set` source containers.

The `combinations` class template is parameterized identically to the source `std::set`.
If any non-default `Compare` or `Allocator` parameters are used for the source `std::set`,
then they must also be specified for a `combinations` instance over it.  

The `combinations` class and iterator are designed for finite source collections.
They will not work well (at all?) with infinite source collections.
