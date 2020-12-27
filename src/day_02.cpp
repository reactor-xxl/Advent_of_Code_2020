#include <fstream>
#include <iostream>
#include <algorithm> /* std::count() */
#include "String.hpp"


struct PasswordPolicy
{
	int		minCount{0};
	int		maxCount{0};
	char	requiredLetter{ ' ' };
};

std::istream& operator>>( std::istream& is, PasswordPolicy& policy );
bool isValid_old_job( const String& password, const PasswordPolicy& policy );
bool isValid_OTCAS( const String& password, const PasswordPolicy& policy );



void	day_02_problem_01( const String& inputFilename )
{
	std::cout << "---- Day 2, Problem 1:\n";

	std::fstream   infile{ inputFilename.c_str() };

	if ( !infile.is_open() )
	{
		std::cout << "Unable to open input file \"" << inputFilename
			<< "\".  day_02_problem_01 solution cannot be provided."
			<< std::endl;
		return;
	}

	PasswordPolicy	policy;
	String			password;
	size_t		validPasswords{ 0 };
size_t		totalPasswords{ 0 };

while ( (infile >> policy) && (infile >> password) )
{
	++totalPasswords;

	if ( isValid_old_job( password, policy ) )
	{
		++validPasswords;
	}
}

std::cout << "Password file \"" << inputFilename
<< "\" had " << validPasswords << " valid passwords out of " << totalPasswords
<< " (" << totalPasswords - validPasswords << " invalid)."
<< std::endl;
}



void	day_02_problem_02( const String& inputFilename )
{
	std::cout << "---- Day 2, Problem 2:\n";

	std::fstream   infile{ inputFilename.c_str() };

	if ( !infile.is_open() )
	{
		std::cout << "Unable to open input file \"" << inputFilename
			<< "\".  day_02_problem_02 solution cannot be provided."
			<< std::endl;
		return;
	}

	PasswordPolicy	policy;
	String			password;
	size_t		validPasswords{ 0 };
	size_t		totalPasswords{ 0 };

	while ( (infile >> policy) && (infile >> password) )
	{
		++totalPasswords;

		if ( isValid_OTCAS( password, policy ) )
		{
			++validPasswords;
		}
	}

	std::cout << "Password file \"" << inputFilename
		<< "\" had " << validPasswords << " valid passwords out of " << totalPasswords
		<< " (" << totalPasswords - validPasswords << " invalid)."
		<< std::endl;
}

std::istream& operator>>( std::istream& is, PasswordPolicy& policy )
{
	auto position = is.tellg();
	char hyphen;
	char colon;
	PasswordPolicy	temp;

	if ( (is >> temp.minCount)
		&& (is >> hyphen)
		&& (hyphen == '-')
		&& (is >> temp.maxCount)
		&& (is >> temp.requiredLetter)
		&& (is >> colon)
		&& (colon == ':')
		)
	{
		policy = temp;
	} else
	{
		is.seekg( position );
	}

	return is;
}


bool isValid_old_job( const String& password, const PasswordPolicy& policy )
{
	auto lcount = std::count( password.begin(), password.end(), policy.requiredLetter );

	if ( (lcount >= policy.minCount) && (lcount <= policy.maxCount) )
		return true;

	return false;
}


bool isValid_OTCAS( const String& password, const PasswordPolicy& policy )
{
	const auto pos1 = policy.minCount - 1;
	const auto pos2 = policy.maxCount - 1;
	const auto letter = policy.requiredLetter;

	if ( (password[pos1] == letter) || (password[pos2] == letter) )
	{
		if ( !((password[pos1] == letter) && (password[pos2] == letter)) )
		{
			return true;
		}
	}
	return false;
}

