#pragma once
#include <vector>
#include <tuple>
//#include <exception>
#include "String.hpp"

#include <istream>

using IndexPair = std::pair<size_t, size_t>;

class ExpenseReport
{
public:
//	ExpenseReport();
//	~ExpenseReport();
	size_t	load(std::istream& inStream );

	bool	find_sum_to( int value, IndexPair& ip );

private:
	std::vector<int>	m_expenses{};

};



