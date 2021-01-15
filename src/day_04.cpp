#include <map>
#include <vector>
#include <sstream> /* validation rules */
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "helpers.hpp"

using Credentials = std::map<std::string, std::string>;
using ValidationRuleset = std::map<std::string, bool (*)(const std::string&)>;

/* solutions */
void	day_04_problem_01();
void	day_04_problem_02();

/* reads a single set of credentials, stopping at a double newline */
static Credentials	read( std::istream& iss );

/* reads and builds a vector of credentials */
static std::vector<Credentials>	buildCredentialsVect( std::istream& iss  );

/* Credentials validation for part one (key/value exists)*/
static bool isValid_basic( const Credentials& doc );

/* Credentials validation for part two (key/value exists, value falls into correct range */
static bool isValid_complete( const Credentials& doc );

/* Credential field validation functions */
static bool byr_rule( const std::string& str );
static bool iyr_rule( const std::string& str );
static bool eyr_rule( const std::string& str );
static bool hgt_rule( const std::string& str );
static bool hcl_rule( const std::string& str );
static bool ecl_rule( const std::string& str );
static bool pid_rule( const std::string& str );
static bool cid_rule( const std::string& str );

const ValidationRuleset ruleset =
{
	{"byr", &byr_rule},
	{"iyr", &iyr_rule},
	{"eyr", &eyr_rule},
	{"hgt", &hgt_rule},
	{"hcl", &hcl_rule},
	{"ecl", &ecl_rule},
	{"pid", &pid_rule},
	{"cid", &cid_rule},
};





void	day_04_problem_01()
{
	std::ifstream   infile;

	if ( !begin_problem( 4, 1, infile ) )
		return;


	auto allDocs = buildCredentialsVect( infile );
	auto validCount = 0;
	auto invalidCount = 0;

	for ( const auto& d : allDocs )
	{
		if ( isValid_basic( d ) )
			++validCount;
		else
			++invalidCount;

	}

	std::cout << "There were " << validCount << " valid documents and " << invalidCount << " invalid ones." << std::endl;
}


void	day_04_problem_02()
{
	std::ifstream   infile;

	if ( !begin_problem( 4, 2, infile ) )
		return;

	auto allDocs = buildCredentialsVect( infile );
	auto validCount = 0;
	auto invalidCount = 0;

	for ( const auto& d : allDocs )
	{
		if ( isValid_complete( d ) )
			++validCount;
		else
			++invalidCount;

	}

	std::cout << "There were " << validCount << " valid documents and " << invalidCount << " invalid ones." << std::endl;
}





Credentials	read( std::istream& iss )
{
	Credentials doc;
	bool inDoc = true;

	while ( iss && inDoc )
	{
		std::string key;
		std::string value;
		char colon;
		char ws;

		std::skipws( iss );

		iss >> std::setw( 3 ) >> key;
		iss >> colon;
		iss >> value;
		doc[key] = value;

		std::noskipws( iss );
		iss >> ws;
		if ( ws == '\n' )
		{
			if ( iss.peek() == '\n' )
			{
				/* two newlines in a row, next Document */
				inDoc = false;
			}
		}
	}
	
	if ( !doc.count( "cid" ) )
		doc["cid"];

	return doc;
}


std::vector<Credentials>	buildCredentialsVect( std::istream& iss  )
{
	std::vector<Credentials> allDocs;

	while ( iss )
	{
		Credentials doc = read(iss);
		allDocs.push_back( doc );
	}
	return allDocs;
}


bool isValid_basic( const Credentials& doc )
{

	for ( const auto& rule : ruleset )
	{
		if ( !doc.count( rule.first ) )
			return false;
	}

	return true;
}


bool isValid_complete( const Credentials& doc )
{

	for ( const auto& rule : ruleset )
	{
		if ( !doc.count( rule.first ) )
			return false;

		const auto& docValue = doc.at( rule.first );

		if ( rule.second( docValue ) )
			continue;
		else
			return false;
	}

	return true;
}





bool byr_rule( const std::string& str )
{
	/* byr (Birth Year) - four digits; at least 1920 and at most 2002 */
	const int earliestYear = 1920;
	const int latestYear = 2002;

	std::istringstream iss{ str };
	int year{ 0 };

	iss >> year;

	if ( (year >= earliestYear) && (year <= latestYear) )
	{
		return true;
	}

	return false;
}


bool iyr_rule( const std::string& str )
{
	/* iyr (Issue Year) - four digits; at least 2010 and at most 2020 */
	const int earliestYear = 2010;
	const int latestYear = 2020;

	std::istringstream iss{ str };
	int year{ 0 };

	iss >> year;

	if ( (year >= earliestYear) && (year <= latestYear) )
	{
		return true;
	}

	return false;
}


bool eyr_rule( const std::string& str )
{
	/* eyr (Expiration Year) - four digits; at least 2020 and at most 2030 */
	const int earliestYear = 2020;
	const int latestYear = 2030;

	std::istringstream iss{ str };
	int year{ 0 };

	iss >> year;

	if ( (year >= earliestYear) && (year <= latestYear) )
	{
		return true;
	}

	return false;
}


bool hgt_rule( const std::string& str )
{
	/* hgt (Height) - a number followed by either cm or in:
    If cm, the number must be at least 150 and at most 193.
    If in, the number must be at least 59 and at most 76 */
	const int cmMin = 150;
	const int cmMax = 193;

	const int inMin = 59;
	const int inMax = 76;

	std::istringstream iss{ str };

	int height{ 0 };
	std::string unit;

	iss >> height;
	iss >> unit;

	if ( unit == std::string{ "cm" } )
	{
		if( (height >= cmMin) && (height <= cmMax) )
		{
			return true;
		}
	}
	else if ( unit == std::string{ "in" } )
	{
		if( (height >= inMin) && (height <= inMax) )
		{
			return true;
		}
	}

	return false;
}

bool hcl_rule( const std::string& str )
{
	/* hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f */
	std::istringstream iss{ str };

	char pound{ ' ' };

	iss >> pound;

	if ( pound != '#' )
		return false;

	std::string color;

	iss >> color;

	if ( color.size() != 6 )
		return false;

	for ( const auto c : color )
	{
		if ( (c >= '0') && (c <= '9') )
		{
			/* valid decimal */
		}
		else if ( (c >= 'a') && (c <= 'f') )
		{
			/* valid hex */
		}
		else
			return false;
	}
	return true;
}


bool ecl_rule( const std::string& str )
{
	/* ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth */
	const std::string validEyeColors[]{
		{"amb"},
		{"blu"},
		{"brn"},
		{"gry"},
		{"grn"},
		{"hzl"},
		{"oth"},
	};

	std::istringstream iss{ str };

	std::string eyeColor;

	iss >> eyeColor;

	for ( const auto& color : validEyeColors )
	{
		if ( color == eyeColor )
		{
			return true;
		}
	}

	return false;
}


bool pid_rule( const std::string& str )
{
	/* pid (Passport ID) - a nine-digit number, including leading zeroes.*/
	std::istringstream iss{ str };

	std::string pidNum;

	iss >> pidNum;

	if ( pidNum.size() != 9 )
		return false;

	for ( auto c : pidNum )
	{
		if ( !isdigit( c ) )
			return false;
	}

	return true;
}


bool cid_rule( const std::string& str )
{
	/* cid (Country ID) - ignored, missing or not. */
	if( str.size() || !str.size() )
		return true; return true;
}

