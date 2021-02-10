#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

const std::string test_joltages1{
R"go(16
10
15
5
1
11
7
19
6
12
4)go"
};


const std::string test_joltages2{
R"go(28
33
18
42
31
14
46
20
48
47
24
23
49
45
19
38
39
11
1
32
25
35
8
17
7
9
4
2
34
10
3)go"
};


template <typename T>
std::vector<T>	read_numbers( std::istream& is )
{
	T val;
	std::vector<T> numbers;
	while ( is >> val )
	{
		numbers.push_back( val );
	}

	return numbers;
}


void	day_10_test()
{
	// test_joltages
	std::istringstream iss{ test_joltages2 };

	auto joltages = read_numbers<int>( iss );

	if ( !joltages.size() )
	{
		std::cout << "Failed to read the adapter joltage ratings." << std::endl;
		return;
	}

	joltages.push_back( 0 ); // receptacle

	std::sort( joltages.begin(), joltages.end() );

	joltages.push_back( joltages[joltages.size() - 1] + 3 );

	std::map<int, int> spreadCount;

	spreadCount[1] = 0;
	spreadCount[2] = 0;
	spreadCount[3] = 0;


	auto lastIndex = joltages.size() - 1;

	for ( auto i = 0; i < lastIndex; ++i )
	{
		auto diff = joltages[i + 1] - joltages[i];

		++spreadCount[diff];

		std::cout << "Diff: " << diff << "\n";
	}

	for ( auto diff : spreadCount )
	{
		std::cout << "There were " << diff.second << " differences of " << diff.first << " jolts.\n";
	}

}




