/**
 *	@file      combination_iterator.h
 *	@author    John Szwast
 *	@year      2014
 *	@copyright none
 */


namespace szwast
{
	template<typename K, class ... args>
	class combinations
	{
		typename template<K, args> Container;
		typename Container::const_iterator;

	public:
		combinations(Container::const_iterator begin, Iter end, size_t size);

	private:

	};
}
