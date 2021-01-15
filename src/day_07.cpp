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

std::vector<BagRule>	gather_bag_rules( std::istream& is )
{
	std::vector<BagRule>	rules;

	std::string ruleLine;

	while ( std::getline( is, ruleLine ) )
	{
		BagRule br;
		if ( build_bag_rule( ruleLine, br ) )
		{
			rules.push_back( br );
		}
	}

	return rules;
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


void	day_07_problem_01()
{
	std::ifstream   infile;

	if ( !begin_problem( 7, 1, infile ) )
		return;

	auto bagRules = gather_bag_rules( infile );

	for ( const auto& br : bagRules )
	{
		debug_dump_BagRule( br );
	}
}


