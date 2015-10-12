#Power Set Iterator

##Feature

###Iterate over a power set

The Power Set Iterator shall be capable of iterating
over the power set of an STL container.

###Not generate power set

The Power Set Iterator shall not require
nor internally perform the creation of
the power set of the source container.

##Compatibility

###Source container

A Power Set Iterator shall be able to be created
from any STL-compatible container, not just `std::set`.

###Range

Iterators equivalent to `std::begin` and `std::end` shall be able to be created.

The Power Set Iterator shall support the use of the range for syntax
for iterating over the power set.

###Iterator usage

The Power Set Iterator shall meet the requirements of a standard `ForwardIterator`.
