#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>



std::string		inputFilename( int day )
{
	std::ostringstream oss;

	oss << "input\\day_" << std::setw( 2 ) << std::setfill( '0' ) << day << ".txt";

	return oss.str();
}



bool begin_problem( int day, int part, std::ifstream& infile )
{

	std::cout << "Day " << day << ", Problem " << part << ":\n";
	auto filename = inputFilename( day );
	infile.open( filename.c_str() );

	if ( !infile.is_open() )
	{
		std::cout << "Unable to open input file \"" << filename
			<< "\".  Day " << day << ", Problem " << part << " solution cannot be provided."
			<< std::endl;

		return false;
	}

	return true;
}


std::vector<std::string>	string_split( const std::string& str, char delim )
{
	std::istringstream iss{ str };

	std::string subString;

	std::vector<std::string> splittedString;

	while ( std::getline( iss, subString, delim ) )
	{
		splittedString.push_back( subString );
	}

	return splittedString;
}




std::vector<std::string>	string_split_ignore_empty( const std::string& str, char delim )
{
	std::istringstream iss{ str };

	std::string subString;

	std::vector<std::string> splittedString;

	while ( std::getline( iss, subString, delim ) )
	{
		if ( subString.length() )
		{
			splittedString.push_back( subString );
		}
	}

	return splittedString;
}


