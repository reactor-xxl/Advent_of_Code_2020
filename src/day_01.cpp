#include <vector>
#include <fstream>
#include "day_01.hpp"


size_t		ExpenseReport::load( std::istream& inStream )
{
	m_expenses.clear();

	if ( inStream )
	{
		int expense;
		while ( inStream >> expense )
		{
			m_expenses.push_back( expense );
		}
	}
	else
	{
		/* failed to open expenses stream */
	}

	return m_expenses.size();
}

bool	ExpenseReport::find_sum_to( int value, IndexPair& ip )
{
	if ( m_expenses.size() < 2 )
	{
		return false;
	}

	for ( auto i = 0; i < m_expenses.size(); ++i )
	{
		for ( auto j = i + 1; j < m_expenses.size(); ++j )
		{
			if ( m_expenses[i] + m_expenses[j] == value )
			{
				ip.first = i;
				ip.second = j;
				return true;
			}
		}
	}

	return false;
}

