
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "String.hpp"

using Document = std::map<String, String>;

const String test_data =
R"foo(ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm
)foo";


const String more_test_data =
R"foo(ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in)foo";


void DEBUG_dump_Document( const Document& doc );
Document	read( std::istream& iss );
void	buildDocumentList( const String& str );

bool byr_rule( const String& str );
bool iyr_rule( const String& str );
bool eyr_rule( const String& str );
bool hgt_rule( const String& str );
bool hcl_rule( const String& str );
bool ecl_rule( const String& str );
bool pid_rule( const String& str );
bool cid_rule( const String& str );


using ValidationRuleset = std::map<String, bool (*)(const String&)>;

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


bool isValid( const Document& doc )
{
	/*
		byr (Birth Year)
		iyr (Issue Year)
		eyr (Expiration Year)
		hgt (Height)
		hcl (Hair Color)
		ecl (Eye Color)
		pid (Passport ID)
		cid (Country ID)
	*/
	if ( doc.count( "byr" ) && doc.count( "iyr" )
		&& doc.count( "eyr" ) && doc.count( "hgt" )
		&& doc.count( "hcl" ) && doc.count( "ecl" )
		&& doc.count( "pid" ) && doc.count( "cid" ) )
	{
		return true;
	}

	return false;
}


void DEBUG_dump_Document( const Document& doc )
{
	std::cout << "---- ";
	if ( !isValid( doc ) )
		std::cout << "in";
	
	std::cout << "valid document:\n";

	for ( const auto& kv : doc )
	{
		std::cout << "    " << kv.first << ":" << kv.second << std::endl;
	}
}


Document	read( std::istream& iss )
{
	Document doc;
	bool inDoc = true;

	while ( iss && inDoc )
	{
		String key;
		String value;
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


std::vector<Document>	buildDocumentList( std::istream& iss  )
{
	std::vector<Document> allDocs;

	while ( iss )
	{
		Document doc = read(iss);
		allDocs.push_back( doc );
	}
	return allDocs;
}


void	day_04_problem_01( const String& inputFilename )
{
 	std::cout << "Day 4, Problem 1:\n";

	std::fstream   infile{ inputFilename.c_str() };

	if ( !infile.is_open() )
	{
		std::cout << "Unable to open input file \"" << inputFilename
			<< "\".  Day 4, Problem 1 solution cannot be provided."
			<< std::endl;
		return;
	}

	auto allDocs = buildDocumentList( infile );
	auto validCount = 0;
	auto invalidCount = 0;

	for ( const auto& d : allDocs )
	{
		if ( isValid( d ) )
			++validCount;
		else
			++invalidCount;

	}

	std::cout << "There were " << validCount << " valid documents and " << invalidCount << " invalid ones." << std::endl;
}


/*


void	read( const String& str )
{
	std::istringstream	iss{ more_test_data };
	std::vector<Document> allDocs;

	while ( iss )
	{
		Document doc = read(iss);
		bool inDoc = true;

		while ( iss && inDoc )
		{
			String key;
			String value;
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
					// two newlines in a row, next Document
					inDoc = false;
				}
			}
		}

		allDocs.push_back( doc );
	}

	for ( const auto& d : allDocs )
		DEBUG_dump_Document( d );

}


*/


bool byr_rule( const String& str )
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


bool iyr_rule( const String& str )
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


bool eyr_rule( const String& str )
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


bool hgt_rule( const String& str )
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
	String unit;

	iss >> height;
	iss >> unit;

	if ( unit == String{ "cm" } )
	{
		if( (height >= cmMin) && (height <= cmMax) )
		{
			return true;
		}
	}
	else if ( unit == String{ "in" } )
	{
		if( (height >= inMin) && (height <= inMax) )
		{
			return true;
		}
	}

	return false;
}

bool hcl_rule( const String& str )
{
	/* hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f */
	std::istringstream iss{ str };

	char pound{ ' ' };

	iss >> pound;

	if ( pound != '#' )
		return false;

	String color;

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


bool ecl_rule( const String& str )
{
	/* ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth */
	const String validEyeColors[]{
		{"amb"},
		{"blu"},
		{"brn"},
		{"gry"},
		{"grn"},
		{"hzl"},
		{"oth"},
	};

	std::istringstream iss{ str };

	String eyeColor;

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


bool pid_rule( const String& str )
{
	/* pid (Passport ID) - a nine-digit number, including leading zeroes.*/
	std::istringstream iss{ str };

	String pidNum;

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


bool cid_rule( const String& str )
{
	/* cid (Country ID) - ignored, missing or not. */
	return true;
}

