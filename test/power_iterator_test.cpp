/**
*	\author    John Szwast
*	\year      2016
*	\copyright MIT
*/


#include "power_iterator.hpp"

#include <gtest/gtest.h>

#include <list>
#include <set>
#include <vector>


template<class C>
class PowerSetConstruction : public testing::Test
{
protected:
	using Container = C;
};

using ContainerTypes = testing::Types<std::set<int>, std::vector<int>, std::list<int>>;

TYPED_TEST_SUITE(PowerSetConstruction, ContainerTypes);


TYPED_TEST(PowerSetConstruction, ConstructionFromSet)
{
	std::set<int> s{ 0,3,-6,12 };
	powerset<std::set<int>::const_iterator> power{ s };
	SUCCEED();  // If it compiles, it passes
}


TYPED_TEST(PowerSetConstruction, ConstructionFromIterators)
{
	Container s{ 0, 4, -4, 8, 15 };
	powerset<Container::const_iterator> test1{ s };
	powerset<Container::const_iterator> test2{ s.cbegin(), s.cend() };
	EXPECT_EQ(test1, test2);
}


TYPED_TEST(PowerSetConstruction, CopyConstruction)
{
	Container s{ 0, 5, 6 };
	powerset<Container::const_iterator> test0{ s };
	powerset<Container::const_iterator> test1{ test0 };
	EXPECT_EQ(test0, test1);
}


TYPED_TEST(PowerSetConstruction, MakePowerSet)
{
	Container s{ 0,3,5 };

	powerset<Container::const_iterator> test1{ s };
	auto test2 = make_powerset(s);
	EXPECT_EQ(test1, test2);

	powerset<Container::const_iterator> test3{ s.begin(), s.end() };
	auto test4 = make_powerset(s.begin(), s.end());
	EXPECT_EQ(test3, test4);

	powerset<Container::const_iterator> test5{ s.cbegin(), s.cend() };
	auto test6 = make_powerset(s.cbegin(), s.cend());
	EXPECT_EQ(test5, test6);
}


TEST(PowerSetEquality, EquallyConstructed)
{
	std::set<int> s1{ 1, 2, 3 };
	std::set<int> s2{ s1 };

	powerset<std::set<int>::const_iterator> test1{ s1.begin(), s1.end() };
	powerset<std::set<int>::const_iterator> test2{ s2.begin(), s2.end() };
	EXPECT_EQ(test1, test2);

	std::set<int> s3{ 1, 2, 4 };
	powerset<std::set<int>::const_iterator> test3{ s1.begin(), s1.end() };
	powerset<std::set<int>::const_iterator> test4{ s3.begin(), s3.end() };
	EXPECT_NE(test3, test4);
}


TEST(PowerSetUtility, Size)
{
	std::set<double> s0{};
	std::set<double> s1{ 0.0,};
	std::set<double> s2{ 0.0, 1.0, 4.0 };

	auto size0 = std::pow(2, s0.size());
	auto size1 = std::pow(2, s1.size());
	auto size2 = std::pow(2, s2.size());

	powerset<std::set<double>::const_iterator> test0{ s0 };
	powerset<std::set<double>::const_iterator> test1{ s1 };
	powerset<std::set<double>::const_iterator> test2{ s2 };

	EXPECT_EQ(size0, (test0.size()));
	EXPECT_EQ(size1, (test1.size()));
	EXPECT_EQ(size2, (test2.size()));

	std::size_t element_count = 0;
	for (auto i = test0.cbegin(); i != test0.cend() && element_count < 25; ++i, ++element_count)
	;
	EXPECT_EQ(size0, element_count);

	element_count = 0;
	for (auto i = test1.cbegin(); i != test1.cend() && element_count < 25; ++i, ++element_count)
	;
	EXPECT_EQ(size1, element_count);

	element_count = 0;
	for (auto i = test2.cbegin(); i != test2.cend() && element_count < 25; ++i, ++element_count)
	;
	EXPECT_EQ(size2, element_count);
}


TEST(PowerSetIteratorConstruction, ConstructionFromPowerSet)
{
	std::set<int> s{ 0,4,-4,8 };
	powerset<std::set<int>::const_iterator> test1{ s };

	auto test_begin = test1.begin();
	auto test_cbegin = test1.cbegin();
	auto test_end = test1.end();
	auto test_cend = test1.cend();

	EXPECT_EQ(test_begin, test_begin);
	EXPECT_EQ(test_cbegin, test_cbegin);
	EXPECT_EQ(test_begin, test_cbegin);

	EXPECT_EQ(test_end, test_end);
	EXPECT_EQ(test_cend, test_cend);
	EXPECT_EQ(test_end, test_cend);

	EXPECT_NE(test_begin, test_end);
	EXPECT_NE(test_cbegin, test_cend);
}


TEST(PowerSetIteratorNavigation, PowerSetOfEmptySet)
{
	std::set<int> s{};
	std::vector<std::vector<int>> expected{ {} };
	powerset<std::set<int>::const_iterator> test{ s };
	EXPECT_TRUE(std::equal(expected.cbegin(), expected.cend(), test.cbegin(), test.cend()));
}


TEST(PowerSetIteratorNavigation, PowerSetOfSingleton)
{
	std::set<int> s{ 0 };
	std::vector<std::vector<int>> expected{ {}, {0} };
	powerset<std::set<int>::const_iterator> test{ s };
	EXPECT_TRUE(std::is_permutation(
		expected.cbegin(),
		expected.cend(),
		test.cbegin(),
		test.cend()
	));
}


TEST(PowerSetIteratorNavigation, PowerSetOfTriple)
{
	std::set<int> s{ 0, 1, 2 };
	std::vector<std::vector<int>> expected{ {}, { 0 }, {1}, {2}, {0,1}, {0,2}, {1,2}, {0,1,2} };
	powerset<std::set<int>::const_iterator> test{ s };
	EXPECT_TRUE(std::is_permutation(
		expected.cbegin(),
		expected.cend(),
		test.cbegin(),
		test.cend()
	));
}

