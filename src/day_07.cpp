#include <string>
#include <istream>
#include <sstream>
#include <tuple>
#include <vector>
#include <iostream>
#include <iomanip>
#include "helpers.hpp"

struct	Bag
{
	std::string		adjective{};
	std::string		color{};
};

struct BagRule
{
	Bag	bagType;
	std::vector<std::pair<int, Bag>>	contents;
};

std::istream&	operator>>( std::istream& is, Bag& bag )
{
	std::string adjective;
	std::string color;
	std::string trailingBag;

	auto pos = is.tellg();

	if ( (is >> adjective) && (is >> color) && (is >> trailingBag)
		&& ((trailingBag == "bag") || (trailingBag == "bags")) )
	{
		/* format correct */
		bag.adjective = adjective;
		bag.color = color;
	}
	else
	{
		is.seekg( pos );
		is.setstate( std::ios_base::failbit );
	}

	return is;
}



std::ostream&	operator<<( std::ostream& os, const Bag& bag )
{
//	os << "{ \"" << bag.adjective << "\", \"" << bag.color << "\" }";

	os << bag.adjective << " " << bag.color << " bag";

	return os;
}



void testBagRead()
{
	std::vector<std::string> bagList{
		{"vibrant beige bags"},
		{"wavy silver bag"},
		{"shiny indigo bags"},
		{"wavy aqua bags"},
		{"mirrored cyan bag"}
	};

	std::cout << "Using a manually built vector of strings:\n";

	for ( const auto& str : bagList )
	{
		Bag tempBag;
		std::istringstream iss{ str };

		iss >> tempBag;

		std::cout << tempBag << std::endl;
	}

	std::cout << "Using a vector built froma a single string:\n";

	std::string bagStr{ "shiny lavender bag, shiny salmon bags,,, bright salmon bags, posh fuchsia bags, plaid chartreuse bags, vibrant bronze bags" };

	auto collector = string_split_ignore_empty( bagStr, ',' );

	for ( const auto& str : collector )
	{
		Bag tempBag;
		std::istringstream iss{ str };

		iss >> tempBag;

		std::cout << tempBag << std::endl;
	}

}


bool build_bag_rule( const std::string& str, BagRule& currentRule )
{
	std::istringstream iss{ str };

	if ( !(iss >> currentRule.bagType) )
	{
		std::cout << "Unable to read initial bag from string \"" << iss.str() << "\"" << std::endl;
		return false;
	}

	std::string wordContain;

	if ( !(iss >> wordContain) || (wordContain != "contain") )
	{
		std::cout << "Invalid format after intial bag with temp == \"" << wordContain << "\"" << std::endl;
		return false;
	}

	std::string restOfLine;

	iss >> std::ws;

	std::getline( iss, restOfLine, '.' );

	if ( restOfLine == "no other bags" )
	{
		currentRule.contents.clear();
		return true;
	}

	// so now we have us a vector of unread bag rules
	auto csvRules = string_split_ignore_empty( restOfLine, ',' );

	for ( const auto& ruleStr : csvRules )
	{
		std::istringstream ruleIss{ ruleStr };
		int count;
		Bag tempBag;

		if ( (ruleIss >> count) && (ruleIss >> tempBag) )
		{
			// good read of a rule:
			currentRule.contents.push_back( std::make_pair( count, tempBag ) );
		}
		else
		{
			std::cout << "Problem reading rule." << std::endl;
			return false;
		}
	}

	return true;
}

void debug_dump_BagRule( const BagRule& bagrule )
{
	std::cout << bagrule.bagType << "s contain";

	if ( bagrule.contents.size() == 0 )
	{
		std::cout << " no other bags." << std::endl;
	}
	else
	{
		for ( const auto& bagpair : bagrule.contents )
		{
			std::cout << "\n  " << bagpair.first << " " << bagpair.second;
			if ( bagpair.first > 1 ) std::cout << "s";
		}
	}

	std::cout << std::endl;
}

bool test_build_bag_rule( const std::string& str )
{
	BagRule bRule;
	auto result = build_bag_rule( str, bRule );

	if ( result )
	{
		debug_dump_BagRule( bRule );
	}
	else
	{
		std::cout << "test_build_bag_rule() :: unable to properly read BagRule" << std::endl;
	}

	return result;
}

/*

void read_single_bag_rule()
{
	std::string exampleRule{
		"vibrant beige bags contain 1 wavy silver bag, 4 shiny indigo bags, 2 wavy aqua bags, 1 mirrored cyan bag."
	};

	std::istringstream	ruleStream{ exampleRule };

	Bag	outerBag;
	std::string tempStr;

	std::vector<std::pair<int, Bag>>	contents;

	if ( ruleStream >> outerBag )
	{
		ruleStream >> tempStr;
		if ( tempStr == "contain" )
		{
			int		count;
			char	comma;
			Bag		tempBag;
			do {
				if ( ruleStream >> count )
					std::cout << "read count = " << count << std::endl;

				if ( ruleStream >> tempBag )
					std::cout << "read tempBag = " << tempBag << std::endl;
				else
				{
					std::cout << "void read_single_bag_rule(): ";
					std::cout << "did not read Bag correctly.  " << ruleStream.str() << std::endl;

				}

				contents.push_back( std::make_pair( count, tempBag ) );

			} while ( (ruleStream >> comma) && (comma == ',') );

			if ( comma == '.' )
			{
				std::cout << "end of rule set reached correctly." << std::endl;
			}
			else
			{
				std::cout << "Parsing problems - end of ruleset not correctly determined. comma == '" << comma << "'" << std::endl;
			}

			std::cout << "outer bag is " << outerBag << ", contains:" << std::endl;
			for ( const auto& bp : contents )
			{
				std::cout << "  " << bp.first << " " << bp.second << std::endl;
			}

		}
		else
		{
			std::cout << "Parsing problems.  tempStr == \"" << tempStr << "\", tempBag == " << outerBag << std::endl;
		}
	}



}

*/

/*
BagRule	read_split_bag_rule(std::istream& is )
{
	std::cout << "void read_split_bag_rule() : BEGIN" << std::endl;


	std::string ruleLine;
	std::getline( is, ruleLine, '.' );

	std::istringstream	ruleStream{ ruleLine };

	std::string tempStr;
	BagRule br;


	std::vector<std::pair<int, Bag>>	contents;

	if ( !(ruleStream >> br.bagType ) )
	{
		std::cout << "Unable to read outermost bag." << std::endl;
		return;
	}

	if ( !(ruleStream >> tempStr) || (tempStr != "contain") )
	{
		std::cout << "Parsing problems.  Read outermost bag, but not keyword 'contain'.  tempStr == \"" << tempStr << std::endl;
		return;
	}

	std::getline( ruleStream, tempStr, '.' );

	if ( tempStr == " no other bags" )
	{
		// * bottom level bag * /
	}

	auto splitString = string_split( tempStr, ',' );

	for ( const auto& str : splitString )
	{
		int		count{ -1 };
		Bag		tempBag;
		std::istringstream iss{ str };

		if ( iss >> count )
		{
			std::cout << "read count = " << count << std::endl;
		}
		else {
			std::cout << "did not read count correctly.  iss = \"" << iss.str() << "\"" << std::endl;
			return;
		}

		if ( iss >> tempBag )
		{
			std::cout << "read tempBag = " << tempBag << std::endl;
		}
		else {
			std::cout << "did not read Bag correctly.  iss = \"" << iss.str() << "\"" << std::endl;
			return;
		}

		br.contents.push_back( std::make_pair( count, tempBag ) );
	}


	std::cout << "Outermost bag (rule bagType) " << br.bagType << " contains:\n";

	for ( const auto& bp : br.contents )
	{
		std::cout << "  " << bp.first << " " << bp.second << std::endl;
	}


	std::cout << "void read_split_bag_rule() : COMPLETE" << std::endl;
	return br;
}


*/
/*

BAG(S) contain BAGLIST
where BAGLIST is one or more
%COUNT% BAG(S) [, %COUNT% BAG(S)...]

vibrant beige bags contain 1 wavy silver bag, 4 shiny indigo bags, 2 wavy aqua bags, 1 mirrored cyan bag.

striped orange bags contain 1 vibrant green bag, 5 plaid yellow bags, 1 drab magenta bag.

{$modifier} {$color} bags contain %d

WHERE bag is
{$modifier} {$color} bag(s)


*/


