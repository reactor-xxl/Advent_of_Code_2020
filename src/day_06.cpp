
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <istream>
#include <sstream>

#include <map>
#include "helpers.hpp"

/*
#include <map>
using SolutionMap = std::map<int, void(*)(const std::string&)>;
*/

const std::string d6_test_data =
R"str(abc

a
b
c

ab
ac

a
a
a
a

b)str";

using GroupAnswers = std::map<char, int>;

size_t	read_group_answers( std::istream& inFile, GroupAnswers& answers)
{
	std::string temp;
	size_t	groupSize{ 0 };

	std::ws( inFile );

	while ( std::getline( inFile, temp ) && temp.size() )
	{
		for ( auto c : temp )
		{
			++answers[c];
		}
		++groupSize;
	}

	return groupSize;
}


void DEBUG_dump_answers( const GroupAnswers& ga )
{
	std::cout << ga.size() << " unique answers:" << std::endl;

	for ( const auto& pair : ga )
	{
		std::cout << "answers['" << pair.first << "'] == " << pair.second << std::endl;
	}
}


void	day_06_problem_01()
{
	std::ifstream   infile;

	if ( !begin_problem( 6, 1, infile ) )
		return;

	auto accumulator = 0;

	while ( infile )
	{
		GroupAnswers answers;
		read_group_answers( infile, answers);
		accumulator += answers.size();
	}
	std::cout << "The sum of the count of unique answers in each group is " << accumulator << std::endl;
}

void	day_06_problem_02()
{
	std::ifstream   infile;

	if ( !begin_problem( 6, 2, infile ) )
		return;

	auto accumulator = 0;

	while ( infile )
	{
		GroupAnswers answers;
		auto	groupSize = read_group_answers( infile, answers);

		for ( const auto& pair : answers )
		{
			if ( pair.second == groupSize )
			{
				/* everyone answered that question */
				++accumulator;
			}
		}
	}
	std::cout << "The sum of the intersection of answers from each group is " << accumulator << std::endl;
}








