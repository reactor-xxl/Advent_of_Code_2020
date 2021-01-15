/*****************************************************************************
                               MIT License

Copyright (c) 2020 December 27	Jonathon Rogers

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/
#include "solutions.hpp"
#include "helpers.hpp"
#include <iostream>
#include <iomanip>

void read_single_bag_rule();
void testBagRead();

void read_split_bag_rule();

bool test_build_bag_rule( const std::string& str );


int main()
{
/*
    day_01_problem_01();
    day_01_problem_02();

    std::cout << '\n';

    day_02_problem_01( "input\\day_02.txt" );
    day_02_problem_02( "input\\day_02.txt" );

    std::cout << '\n';

    day_03_problem_01( "input\\day_03.txt" );
    day_03_problem_02( "input\\day_03.txt" );

    std::cout << '\n';

    day_04_problem_01("input\\day_04.txt");
    day_04_problem_02("input\\day_04.txt");

    std::cout << '\n';

    day_05_problem_01("input\\day_05.txt");
    day_05_problem_02("input\\day_05.txt");

    std::cout << '\n';
*/
/*
    read_split_bag_rule();

	std::string exampleRule{
		"vibrant beige bags contain 1 wavy silver bag, 4 shiny indigo bags, 2 wavy aqua bags, 1 mirrored cyan bag."
	};
*/
    std::string test1{ "shiny lavender bags contain 2 shiny salmon bags, 2 bright salmon bags, 4 posh fuchsia bags." };

    test_build_bag_rule( test1 );

    std::string test2{"bright chartreuse bags contain no other bags."};

    test_build_bag_rule( test2 );

 //   testBagRead();
 //   read_single_bag_rule();
//    testBagRead();

    return 0;
}




