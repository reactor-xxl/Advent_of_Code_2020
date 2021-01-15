#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm> /* std::sort */
#include <string>
#include "helpers.hpp"


/* solutions */
void	day_05_problem_01();
void	day_05_problem_01();

/* performs BSP across the value range [low,high] based on the lo/hi chars from sequence.
	Characters that are not the lo/hi chars in sequence are ignored*/
static int	converge( int low, int high, char loChar, char hiChar, const std::string& sequence );

/* reads a series of BSP sequences and builds a vector of Seat IDs from the sequences. */
std::vector<size_t>		build_seatVect( std::istream& is );

struct Seat
{
	size_t	row{ 0 };
	size_t	column{ 0 };
	Seat() {}
	Seat( const size_t& sid ) : column{ sid % 8 }, row{ sid / 8 } {}
	Seat( const std::string& sequence)
	{
		row = converge( 0, 127, 'F', 'B', sequence );
		column = converge( 0, 7, 'L', 'R', sequence );
	}

	size_t	get_id() const { return 8 * row + column; }
};

std::ostream&	operator<<( std::ostream& os, const Seat& s )
{
	os << "seat is at row " << std::setw(3) << s.row
		<< ", column " << s.column << " (id: " << std::setw(3) << s.get_id() << ").";

	return os;
}



void	day_05_problem_01()
{
	std::ifstream   infile;

	if ( !begin_problem( 5, 1, infile ) )
		return;

	std::vector<size_t> seatIds = build_seatVect( infile );

	auto size = seatIds.size();

	if ( size )
	{
		std::cout << "The highest seat id was " << seatIds[size - 1] << std::endl;
	}
	else
	{
		std::cout << "There was a problem reading the seat IDs" << std::endl;
	}
}


void	day_05_problem_02()
{
	std::ifstream   infile;

	if ( !begin_problem( 5, 2, infile ) )
		return;


	std::vector<size_t> seatIds = build_seatVect(infile);

	size_t lastIndex = seatIds.size() - 1;

	size_t gapSeat = 0;

	for ( auto i = 0; i < lastIndex; ++i )
	{
		auto diff = seatIds[i + 1] - seatIds[i];
		if ( diff > 1 )
		{
			std::cout << "Gap between seat id " << seatIds[i] << " and " << seatIds[i + 1] << std::endl;
			gapSeat = seatIds[i + 1] - 1;
		}
	}
	
	if ( gapSeat )
		std::cout << "Your seat id is " << gapSeat << std::endl;
	else
		std::cout << "A seat matching the specified parameters could not be found." << std::endl;

}


int	converge( int low, int high, char loChar, char hiChar, const std::string& sequence )
{
	int result = low;

	for ( auto c : sequence )
	{
		if ( c == loChar )
		{
			result = high = (high + low) / 2;
		}
		else if ( c == hiChar )
		{
			result = low = (high + low) / 2 + 1;
		}
		else
		{
			/* ignore out of sequence char */
		}
	}
	return result;
}



std::vector<size_t>		build_seatVect( std::istream& is )
{
	std::string sequence;
	std::vector<size_t> seatIds;
	seatIds.reserve( 1024 );

	while ( is >> sequence )
	{
		Seat s{ sequence };

		seatIds.push_back( s.get_id() );
	}

	std::sort( seatIds.begin(), seatIds.end() );

	return seatIds;
}
