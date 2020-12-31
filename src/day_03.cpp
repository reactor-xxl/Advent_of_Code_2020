#include <vector>
#include <iostream>
#include <fstream>
#include "String.hpp"

//------
#include <sstream>
#include <tuple>

using Slope = std::tuple<int, int>;

class TobogganMap
{
public:
	size_t	width() const;
	size_t	height() const;

	size_t	treeCount() const;
	size_t	stepsTaken() const;

	void	load( std::istream& is );
	bool	traverse(const Slope& s);
	std::vector<int>	traverseAccumulate( const std::vector<Slope>& svec );

	static const String test_map;

private:
	void	resetCounts();
	bool	validate();

	std::vector<String> m_map{};
	size_t		m_treeCount{0};
	size_t		m_blankCount{0};
	size_t		m_stepCount{0};
};


const String TobogganMap::test_map =
R"foo(..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#)foo";


size_t	TobogganMap::width() const
{
	if ( height() )
		return m_map[0].size();
	else
		return 0;
}

size_t	TobogganMap::height() const
{
	return m_map.size();
}

size_t	TobogganMap::treeCount() const
{
	return m_treeCount;
}

size_t	TobogganMap::stepsTaken() const
{
	return m_stepCount;
}

void	TobogganMap::load( std::istream& is )
{
	m_map.clear();
	resetCounts();

	String temp;

    while ( is )
    {
        std::getline( is, temp, '\n' );

		if ( !temp.size() )
		{
			continue;
		}
		else
		{
			m_map.push_back( temp );
		}
    }
}

std::vector<int>	TobogganMap::traverseAccumulate( const std::vector<Slope>& svec )
{
	std::vector<int> results;

	for ( const auto& s : svec )
	{
		if ( traverse( s ) )
		{
			results.push_back( treeCount() );
			std::cout << "Slope (" << std::get<0>(s) << ", "  << std::get<1>(s)
				<< ") will hit " << treeCount() << " trees." << std::endl;
		}
		else
		{
			std::cout << "There was a problem with the map or routing process." << std::endl;
			results.clear();
			return results;
		}
	}

	return results;
}

bool	TobogganMap::traverse(const Slope& s)
{
	resetCounts();

	if ( !validate() )
	{
		std::cout << "TobogganMap::traverse(size_t,size_t) cannot work on an invalid map." << std::endl;
		return false;
	}

	auto rightStep = std::get<0>(s);
	auto downStep  = std::get<1>(s);

	auto lastRow = height() - 1;

	auto currentRow = 0;
	auto currentCol = 0;

	while( currentRow <= lastRow )
	{
		switch ( m_map[currentRow][currentCol] )
		{
		case '.': ++m_blankCount;	break;
		case '#': ++m_treeCount;	break;
		default:/* invalid symbol */
			break;
		}

		++m_stepCount;

		currentRow += downStep;
		currentCol += rightStep;
		currentCol = currentCol % width();
	}
	return true;
}

void	TobogganMap::resetCounts()
{
	m_treeCount		= 0;
	m_blankCount	= 0;
	m_stepCount		= 0;
}

bool	TobogganMap::validate()
{
	if ( !height() || !width() )
	{
		return false;
	}

	auto w = m_map[0].size();

	for ( const auto& str : m_map )
	{
		if(str.size() != w )
			return false;
	}

	return true;
}

/*--------------------------*/

void	day_03_problem_01( const String& inputFilename )
{
 	std::cout << "Day 3, Problem 1:\n";

	std::ifstream	infile{ inputFilename.c_str() };

	if ( !infile.is_open() )
	{
		std::cout << "Unable to open input file \"" << inputFilename
			<< "\".  Day 3, Problem 1 solution cannot be provided."
			<< std::endl;
		return;
	}

	TobogganMap		tb;

	tb.load( infile );

	if ( tb.traverse( Slope{ 3, 1 } ) )
	{
		std::cout << "This route will hit " << tb.treeCount() << " trees." << std::endl;
	}
	else
	{
		std::cout << "There was a problem with the map or routing process." << std::endl;
	}
}



void	day_03_problem_02( const String& inputFilename )
{
 	std::cout << "Day 3, Problem 2:\n";

	std::ifstream	infile{ inputFilename.c_str() };

	if ( !infile )
	{
		std::cout << "Unable to open input file \"" << inputFilename
			<< "\".  Day 3, Problem 2 solution cannot be provided."
			<< std::endl;
		return;
	}

	std::vector<Slope>	svec;

	svec.push_back( Slope{1, 1} );
	svec.push_back( Slope{3, 1} );
	svec.push_back( Slope{5, 1} );
	svec.push_back( Slope{7, 1} );
	svec.push_back( Slope{1, 2} );

	TobogganMap			tb;
	tb.load( infile );

	std::vector<int>	results;

	results = tb.traverseAccumulate( svec );

	if ( !results.size() )
	{
		std::cout << "Unable to accumulate a meaningful result." << std::endl;
		return;
	}

	long long product{ 1 };

	for ( const auto i : results )
	{
		product *= i;
	}

	std::cout << "Total product is " << product << std::endl;
}
