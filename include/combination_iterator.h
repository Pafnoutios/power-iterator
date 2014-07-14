/**
 *	@file      combination_iterator.h
 *	@author    John Szwast
 *	@year      2014
 *	@copyright none
 */


 #include <memory>


namespace szwast
{
	template<typename Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
	class combinations
	{
	public:
		using value_type      = std::set<Key, Compare, Allocator>;
		using allocator_type  = Allocator;
		using size_type       = std::size_t;

		using source_iterator = typename value_type::const_iterator;

		class const_iterator;

		combinations(source_iterator source_begin, source_iterator source_end, size_type size);

		const_iterator begin();
		const_iterator end();

	private:
		source_iterator m_begin;
		source_iterator m_end;
		size_type m_size;
	};

	template<typename Key, class Compare, class Allocator>
	class combinations<Key, Compare, Allocator>::const_iterator
	{
	public:
		using source_iterator = combinations<Key, Compare, Allocator>::source_iterator;

		const_iterator(source_iterator source_begin, source_iterator source_end, size_type size);

	};
}


template<typename Key, class Compare, class Allocator>
szwast::combinations<Key, Compare, Allocator>::combinations(source_iterator source_begin, source_iterator source_end, size_type size)
:	m_begin(source_begin)
,	m_end(source_end)
,	m_size(size)
{

}


