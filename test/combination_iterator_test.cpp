/**
*	\author    John Szwast
*	\year      2014-2016
*	\copyright MIT
*/


#include "gtest/gtest.h"
#include "combination_iterator.hpp"
#include <set>


TEST(combinations_construction, construction_from_set)
{
	std::set<int> s{ 0, 3, -6, 12 };
	for (size_t i = 0; i <= s.size(); ++i)
	{
		combinations<int> test{ s, i };
	}
	SUCCEED();  // If it compiles, it passes.
}


TEST(combinations_construction, construction_from_iterators)
{
	std::set<int> s{0, 4, -4, 8, 15};
	for(size_t i = 0; i <= s.size(); i++)
	{
		combinations<int> test1{ s, i };
		combinations<int> test2{s.cbegin(), s.cend(), i};
		EXPECT_EQ(test1, test2);
	}
}


TEST(CombinationsConstruction, CopyConstruction)
{
	std::set<int> s{0, 3, 5};
	for (std::size_t i = 0; i <= s.size(); ++i)
	{
		combinations<int> test0(s, i);
		combinations<int> test1(test0);
		EXPECT_EQ(test0, test1);
	}
}


TEST(CombinationsConstruction, MakeCombinations)
{
	std::set<int> s{ 0,3,5 };

	combinations<int> test1{ s, 2 };
	auto test2 = make_combinations(s, 2);
	EXPECT_EQ(test1, test2);

	combinations<int> test3{ s.begin(), s.end(), 3 };
	auto test4 = make_combinations(s.begin(), s.end(), 3);
	EXPECT_EQ(test3, test4);

	combinations<int> test5{ s.cbegin(), s.cend(), 1 };
	auto test6 = make_combinations(s.cbegin(), s.cend(), 1);
	EXPECT_EQ(test5, test6);
}


TEST(combinations_equality, equivalently_constructed)
{
	std::set<int> s{1, 2, 3};
	for(std::set<int>::size_type i = 0; i <= s.size(); i++)
	{
		combinations<int> test1{s.begin(), s.end(), i};
		combinations<int> test2{s.begin(), s.end(), i};
		EXPECT_EQ(test1, test2);
	}

	for (std::set<int>::size_type i = 0; i <= s.size(); i++)
		for (std::set<int>::size_type j = 0; j < i; j++)
		{
			combinations<int> test1{s.begin(), s.end(), i};
			combinations<int> test2{s.begin(), s.end(), j};
			EXPECT_NE(test1, test2);
		}
}


TEST(combinations_equality, equally_constructed)
{
	std::set<int> s1{1, 2, 3};
	std::set<int> s2{s1};
	for(std::set<int>::size_type i = 0; i <= s1.size(); i++)
	{
		combinations<int> test1{s1.begin(), s1.end(), i};
		combinations<int> test2{s2.begin(), s2.end(), i};
		EXPECT_EQ(test1, test2);
	}

	for (std::set<int>::size_type i = 0; i <= s1.size(); i++)
		for (std::set<int>::size_type j = 0; j < i; j++)
		{
			combinations<int> test1{s1.begin(), s1.end(), i};
			combinations<int> test2{s2.begin(), s2.end(), j};
			EXPECT_NE(test1, test2);
		}

	std::set<int> s3{1, 2, 4};
	for(std::set<int>::size_type i = 0; i <= s1.size(); i++)
	{
		combinations<int> test1{s1.begin(), s1.end(), i};
		combinations<int> test2{s3.begin(), s3.end(), i};
		EXPECT_NE(test1, test2);
	}
}


TEST(combinations_utility, size)
{
	std::set<double> s1{0.0, 1.0, 4.0};
	EXPECT_EQ(1, (combinations<double>{s1.begin(), s1.end(), 0}.size()));
	EXPECT_EQ(3, (combinations<double>{s1.begin(), s1.end(), 1}.size()));
	EXPECT_EQ(3, (combinations<double>{s1.begin(), s1.end(), 2}.size()));
	EXPECT_EQ(1, (combinations<double>{s1.begin(), s1.end(), 3}.size()));
}




TEST(combination_iterator_construction, construction_from_combinations)
{
	std::set<int> s{0, 4, -4, 8};
	combinations<int> test1{s.begin(), s.end(), 2};
	auto test_begin = test1.begin();
	auto test_end = test1.end();
	EXPECT_NE(test_begin, test_end);

	test1 = combinations<int>{s.begin(), s.end(), 4};
	test_begin = test1.begin();
	test_end = test1.end();
	EXPECT_NE(test_begin, test_end);

	test1 = combinations<int>{s.begin(), s.end(), 5};
	test_begin = test1.begin();
	test_end = test1.end();
	EXPECT_EQ(test_begin, test_end);
}


TEST(combination_iterator_navigation, combinations_zero)
{
	std::set<int> s{0, 9, 8};
	combinations<int> test1{s.begin(), s.end(), 0};
	EXPECT_EQ(1, test1.size());

	auto test_it = test1.begin();
	for (auto i = 0; i < s.size(); i++)
		EXPECT_EQ(std::set<int>{}, *test_it++);
}


TEST(combination_iterator_navigation, combinations_zero_to_end)
{
	std::set<int> s{0, 9, 8};
	combinations<int> test{s.begin(), s.end(), 0};
	EXPECT_NE(test.begin(), test.end());

	combinations<int>::size_type element_count = 0;
	for (auto i = test.begin(); (i != test.end()) && (element_count < 10); i++, element_count++)
		;

	EXPECT_EQ(1, element_count);
}


TEST(combination_iterator_navigation, combinations_one)
{
	std::set<int> s{0, 1, 2, 3};
	std::vector<std::set<int>> results{{0}, {1}, {2}, {3}};
	combinations<int> test{s.begin(), s.end(), 1};

	EXPECT_EQ(results.size(), test.size());
	if (results.size() == test.size())
	{
		EXPECT_TRUE(std::equal(results.begin(), results.end(), test.begin(), test.end()));
	}
}


TEST(combination_iterator_navigation, combinations_two)
{
	std::set<int> s{ 0,1,2,3 };
	std::vector<std::set<int>> results{ {0,1},{0,2},{0,3},{1,2},{1,3},{2,3} };
	combinations<int> test{ s.begin(), s.end(), 2 };

	EXPECT_EQ(results.size(), test.size());
	if (results.size() == test.size())
	{
		EXPECT_TRUE(std::equal(results.cbegin(), results.cend(), test.cbegin(), test.cend()));
	}
}


TEST(combination_iterator_navigation, union_to_whole)
{
	std::set<int> s{0, 1, 2, 3};
	std::set<int> union_of_subs;

	combinations<int> test{s.begin(), s.end(), 2};
	for (auto const& subset : test)
	{
		union_of_subs.insert(subset.begin(), subset.end());
	}

	EXPECT_EQ(s, union_of_subs);
}
