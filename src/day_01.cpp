#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "helpers.hpp"

void	day_01_problem_01();
void	day_01_problem_02();


using ExpenseIndexes = std::vector<size_t>;

class ExpenseReport 
{
public:
	size_t			load(std::istream& inStream );
	bool			find_sum_pair( int value );
	bool			find_sum_trio( int value );
	ExpenseIndexes	get_indexes() const;
	int				at( size_t index ) const;
private:
	std::vector<int>	m_expenses{};
	ExpenseIndexes		m_indexes{};
};



size_t	ExpenseReport::load( std::istream& inStream )
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
		/* stream problems */
	}

	return m_expenses.size();
}

bool	ExpenseReport::find_sum_pair( int value )
{
	const size_t reportSize = m_expenses.size();
	m_indexes.clear();

	if ( reportSize < 2 )
	{
		return false;
	}

	for ( auto i = 0UL; i < reportSize; ++i )
	{
		for ( auto j = i + 1; j < reportSize; ++j )
		{
			if ( m_expenses[i] + m_expenses[j] == value )
			{
				m_indexes.push_back(i);
				m_indexes.push_back(j);
				return true;
			}
		}
	}

	return false;
}

bool	ExpenseReport::find_sum_trio( int value )
{
	const size_t reportSize = m_expenses.size();
	m_indexes.clear();

	if ( reportSize < 3 )
	{
		return false;
	}

	for ( auto i = 0UL; i < reportSize; ++i )
	{
		for ( auto j = i + 1; j < reportSize; ++j )
		{
			for ( auto k = j + 1; k < reportSize; ++k )
			{
				if ( m_expenses[i] + m_expenses[j] + m_expenses[k] == value )
				{
					m_indexes.push_back(i);
					m_indexes.push_back(j);
					m_indexes.push_back(k);
					return true;
				}
			}
		}
	}

	return false;
}

ExpenseIndexes	ExpenseReport::get_indexes() const
{
	return m_indexes;
}

int		ExpenseReport::at( size_t index ) const
{
	return m_expenses.at( index );
}

void	day_01_problem_01()
{
	std::ifstream   infile;

	if ( !begin_problem( 1, 1, infile ) )
		return;

    ExpenseReport er;
	const int sumValue{ 2020 };

    er.load( infile );
    if ( er.find_sum_pair( sumValue ) )
    {
        auto p = er.get_indexes();
        auto product = er.at( p[0] ) * er.at( p[1] );
		std::cout << "The product of the expenses that add up to " << sumValue
			<< " is " << product << std::endl;
    }
    else
    {
        std::cout << "Couldn't find a pair of expenses that added up to " << sumValue << std::endl;
    }
}

void	day_01_problem_02()
{
	std::ifstream   infile;

	if ( !begin_problem( 1, 2, infile ) )
		return;

    ExpenseReport er;
	const int sumValue{ 2020 };

    er.load( infile );
    if ( er.find_sum_trio( sumValue ) )
    {
        auto p = er.get_indexes();
        auto product = er.at( p[0] ) * er.at( p[1] ) * er.at( p[2] );
		std::cout << "The product of the expenses that add up to " << sumValue
			<< " is " << product << std::endl;
    }
    else
    {
        std::cout << "Couldn't find a trio of expenses that added up to " << sumValue << std::endl;
    }
}

