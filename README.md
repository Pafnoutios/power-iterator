# power-iterator
This library contains facicilities for iterating over the power set of a collection
as well as over combinations of various size of a collection.

## Power Set
The power set of a collection is the set of all subsets.

## Combinations
The combinations of a collection is the set of all subsets of a particular size.

The `combinations` class is a virtual container representing the combinations of a particular size of a source collection.
A `combinations` object does not actually store all of the combinations,
but merely enough information to generate them.
The iterator will generate them on demand.

## Known limitations
The power set class is not yet written.

The `combinations` class and iterator are disigned for finite source collections.
They will not work well with infinite source collections.
