/**
 *	@file      combination_iterator_test.cpp
 *	@author    John Szwast
 *	@year      2014
 *	@copyright none
 */


#include "gtest/gtest.h"
#include "combination_iterator.h"
#include <set>


TEST(combinations_construction, construction_from_iterators)
{
	combinations<int> test1{std::set<int>{}.begin(), std::set<int>{}.end(), 0};

	std::set<int> s{0, 4, -4, 8};
	for(size_t i = 0; i <= s.size(); i++)
	{
		combinations<int> test2{s.begin(), s.end(), i};
	}

	SUCCEED();	// If it compiles, it passes.
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
	auto test_it = test1.begin();
	for (auto i = 0; i < s.size(); i++)
		EXPECT_EQ(std::set<int>{}, *test_it++);
}


TEST(combination_iterator_navigation, combinations_zero_to_end)
{
	std::set<int> s{0, 9, 8};
	combinations<int> test{s.begin(), s.end(), 0};
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
	auto test_it = test.begin();
	for (auto vector_it = results.begin();
	    (test_it != test.end()) && (vector_it != results.end());
	    test_it++, vector_it++)
	{
		EXPECT_EQ(*vector_it, *test_it);
	}
}