#include <string>
#include <istream>
#include <sstream>
#include <tuple>
#include <vector>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "helpers.hpp"


struct	Bag
{
	std::string		adjective{};
	std::string		color{};
};

struct BagHash
{
	size_t operator()( const Bag& bag ) const noexcept
	{
		std::string temp{ bag.adjective + bag.color };
		return std::hash<std::string>{}(temp);
	}
};

bool operator==( const Bag& lhs, const Bag& rhs )
{
	return (lhs.adjective == rhs.adjective && lhs.color == rhs.color);
}


struct BagRule
{
	Bag	bagType;
	std::vector<std::pair<int, Bag>>	contents;
};


using BagRuleManager = std::unordered_map<Bag, BagRule, BagHash>;



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



bool build_bag_rule( const std::string& str, BagRule& bagRule )
{
	std::istringstream iss{ str };

	BagRule newRule;

	if ( !(iss >> newRule.bagType ) )
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
		bagRule = newRule;
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
			newRule.contents.push_back( std::make_pair( count, tempBag ) );
		}
		else
		{
			std::cout << "Problem reading rule." << std::endl;
			return false;
		}
	}


	bagRule = newRule;
	return true;
}

bool add_bag_rule( const std::string& str, BagRuleManager& ruleCollector )
{
	std::istringstream iss{ str };

	BagRule newRule;

	if ( !(iss >> newRule.bagType ) )
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

	if ( !ruleCollector.count( newRule.bagType ) )
	{
		ruleCollector[newRule.bagType] = newRule;
	}

	std::string restOfLine;

	iss >> std::ws;

	std::getline( iss, restOfLine, '.' );

	if ( restOfLine == "no other bags" )
	{
		ruleCollector[newRule.bagType].contents.clear();
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
			if ( !ruleCollector.count( tempBag ) )
			{
				// bag type not already held, add it with a blank ruleset for now
				ruleCollector[tempBag] = {};
			}

			ruleCollector[newRule.bagType].contents.push_back( std::make_pair( count, tempBag ) );
		}
		else
		{
			std::cout << "Problem reading rule." << std::endl;
			return false;
		}
	}

	return true;
}



BagRuleManager	gather_bag_rules( std::istream& is )
{
	BagRuleManager ruleCollector;

	std::string ruleLine;

	while ( std::getline( is, ruleLine ) )
	{
		BagRule br;
		if ( build_bag_rule( ruleLine, br ) )
		{
			ruleCollector.insert( {br.bagType, br} );
		}
	}

	return ruleCollector;
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
	void debug_dump_bagrule_recursize( const BagRule & br, const BagRuleManager & bagManager );

	std::ifstream   infile;

	if ( !begin_problem( 7, 1, infile ) )
		return;

	auto bagRules = gather_bag_rules( infile );

	debug_dump_bagrule_recursize( bagRules.begin()->second, bagRules );

	return;

	for ( const auto& br : bagRules )
	{
		const auto& rule = br.second;

		for ( const auto& b : rule.contents )
		{
			debug_dump_BagRule( bagRules[b.second] );
		}

		std::cout << "If this works, then we successfully performed a lookup" << std::endl;
		std::cin.get();
		break;
	}

}



struct BagNode
{
	Bag	bag;
	std::vector<BagNode*> contents;
};


void printBagRules( const BagNode *node )
{
	std::cout << node->bag << " contain";

	if ( node->contents.size() == 0 ) {
	std::cout << " no other bags." << std::endl;
	return;
}

	for ( const auto bn : node->contents )
	{
		std::cout << "  ";
		printBagRules( bn );
	}
}


void debug_dump_bagrule_recursize(const BagRule& br, const BagRuleManager& bagManager )
{
	static int indent{ 0 };

	++indent;

	std::cout << std::setw( indent ) << ' ';

	std::cout << br.bagType << " contain";

	if ( br.contents.size() == 0 )
	{
		std::cout << " no other bags." << std::endl;
		--indent;
		return;
	}

	std::cout << "\n";

	for ( const auto& p : br.contents )
	{
		auto it = bagManager.find( p.second );

		std::cout << "  " << p.first << " " << p.second << "\n";
		debug_dump_bagrule_recursize( it->second, bagManager);
	}
}


void debug_dump( const BagRuleManager& bagManager )
{
	for ( const auto& br : bagManager )
	{
		std::cout << br.first << " contain";

		if ( br.second.contents.size() == 0 )
		{
			std::cout << " no other bags." << std::endl;
			continue;
		}

//		std::cout << "  " << br.second.contents.

	}
}