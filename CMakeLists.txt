#	\author    John Szwast
#	\year      2014-2016
#	\copyright MIT

cmake_minimum_required(VERSION 3.2)

project(PowerIterators CXX)

enable_testing()

set(doc README.md spec/CombinationIterator.md spec/PowerSetIterator.md design/Iterators.md)
set(headers include/combination_iterator.hpp include/power_iterator.hpp)
set(sources)
set(unit_tests test/combination_iterator_test.cpp test/power_iterator_test.cpp)

find_package(memoized_member CONFIG)

add_library(PowerIterators INTERFACE)
target_include_directories(PowerIterators INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_link_libraries(PowerIterators INTERFACE memoized_member::memoized_member)

install(TARGETS PowerIterators EXPORT PowerIterators)
install(EXPORT PowerIterators DESTINATION cmake)
install(FILES ${headers} DESTINATION include)
install(FILES ${doc} DESTINATION doc)

find_package(GTest CONFIG)

add_executable(pi_tests ${unit_tests})

target_link_libraries(pi_tests PowerIterators GTest::GTest)

add_test(NAME PowerIterators-Unit-Tests COMMAND pi_tests)
