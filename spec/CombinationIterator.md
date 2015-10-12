#Combination Iterator

##Feature

###Iterate over a set of combinations

The Combination Iterator shall be capable of iterating
over the set of combinations of a specified size of an STL container.

###Not generate set of combinations

The Combination Iterator shall not require
nor internally perform the creation of
the set of combinations of the source container over which it is iterating.

##Compatibility

###Source container

A Combination Iterator shall be able to be created
from any STL-compatible container, not just `std::set`.

###Range

Iterators equivalent to `std::begin` and `std::end` shall be able to be created.

The Combinations Iterator shall support the use of the range for syntax
for iterating over the combinations.

###Iterator usage

The Combination Iterator shall meet the requirements of a standard `ForwardIterator`.
