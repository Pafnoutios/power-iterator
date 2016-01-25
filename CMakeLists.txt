#	\author    John Szwast
#	\year      2014-2016
#	\copyright MIT

cmake_minimum_required(VERSION 2.8)

project(PowerIterators)

add_subdirectory(googletest)

enable_testing()

include_directories(googletest/include memoized-member)

set(doc README.md spec/CombinationIterator.md spec/PowerSetIterator.md design/Iterators.md)
set(headers combination_iterator.hpp power_iterator.hpp)
set(sources)
set(unit_tests combination_iterator_test.cpp power_iterator_test.cpp)


source_group("Documentation" FILES ${doc})
source_group("Header Files" FILES ${headers})
source_group("Source Files" FILES ${sources})
source_group("Unit Tests" FILES ${unit_tests})

add_executable(pi_tests ${headers} ${unit_tests} ${doc})
target_compile_options(pi_tests PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:--std=c++11>)
target_link_libraries(pi_tests gtest_main)

add_test(NAME PowerIterator-Unit-Tests COMMAND pi_tests)
