#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>
#include <map>
#include "helpers.hpp"

/*
operation (acc, jmp, or nop)
*/

const std::string day_08_test_loop_prog{ R"foo(nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)foo" };


const std::string day_08_test_complete_prog{ R"foo(nop +0 
acc +1 
jmp +4 
acc +3 
jmp -3 
acc -99
acc +1 
nop -4 
acc +6)foo" };


enum class Operation
{
	NOP,
	ACC,
	JMP
};

using Argument = int;
using ExecutionCount = int;

using Instruction = std::tuple<ExecutionCount, Operation, Argument>;

using	Program = std::vector<Instruction>;

/* this map is required to make it easier to use a string with a switch/case */
std::map<std::string, Operation>	stringToOperation{
	{"nop", Operation::NOP },
	{"acc", Operation::ACC },
	{"jmp", Operation::JMP },
};

Program		load_program(std::istream& iss)
{
	Program program;
	bool error = false;

	iss.clear();

	if ( !iss )
	{
		std::cout << "load_program() : Warning: the stream is not ready." << std::endl;
	}

	do
	{
		std::string opString;
		Argument	arg;
		
		/* read operation */
		if ( !(iss >> opString) )
		{
			std::cout << "Failed to read the operation." << std::endl;
			error = true;
			break;
		}

		/* read argument */
		if ( !(iss >> arg) )
		{
			std::cout << "Failed to read the argument." << std::endl;
			error = true;
			break;
		}

		/* format correct, perform lookup on instruction */
		if ( !stringToOperation.count(opString) )
		{
			std::cout << "Unrecognized operation \"" << opString << "\"" << std::endl;
			error = true;
			break;
		}

		program.push_back( Instruction{ 0, stringToOperation[opString], arg } );

	} while ( iss.peek() != EOF );

	if ( error )
	{
		std::cout << "Program did not load successfully." << std::endl;
//		program.clear();
	}

	return program;
}



int get_count( const Instruction& instruction )
{
	return std::get<0>( instruction );
}


void increment_count( Instruction& instruction )
{
	++std::get<0>( instruction );
}


auto get_op( const Instruction& instruction )
{
	return std::get<1>( instruction );
}


auto get_arg( const Instruction& instruction )
{
	return std::get<2>( instruction );
}

void debug_dump_program( const Program& prog )
{
	std::cout << "Program size is " << prog.size() << std::endl;

	for ( const auto& line : prog )
	{
		std::cout << "[" << std::noshowpos << std::setw(2) << std::setfill('0') << get_count( line ) << "]  ";
		switch ( get_op( line ) )
		{
		case Operation::NOP: std::cout << "nop";	break;
		case Operation::ACC: std::cout << "acc";	break;
		case Operation::JMP: std::cout << "jmp";	break;
		default:	std::cout << "(unknown instruction)";	 break;
		}

		std::cout << " " << std::showpos << get_arg( line ) << std::endl;
	}

	std::noshowpos( std::cout );
}



bool run_program(Program prog)
{
	if ( !prog.size() )
	{
		std::cout << "This program contains no instructions." << std::endl;
		return false;
	}

	long long accumulator{ 0 };

	size_t step_count{ 0 };

	size_t currentIndex{ 0 };
	size_t lastIndex{ prog.size() - 1 };
	
	bool completed = false;

	bool error_instruction = false;
	bool error_infinite_loop = false;
	bool error_overrun = false;

	while ( !completed && !error_instruction && !!error_infinite_loop && !error_overrun )
	{

		if ( get_count( prog[currentIndex] ) )
		{
			// instruction has been executed before.
			std::cout << "Instruction at prog[" << currentIndex << "] has been executed before." << std::endl;
			error_infinite_loop = true;
			break;
		}
		else
		{
			increment_count( prog[currentIndex] );
		}


		int offset = 1;

		switch ( get_op( prog[currentIndex] ) )
		{
			case Operation::NOP:
				offset = 1;
				break;
			case Operation::ACC:
				offset = 1;
				accumulator += get_arg( prog[currentIndex] ); 
				break;
			case Operation::JMP:
				offset = get_arg( prog[currentIndex] );
				if ( (offset + currentIndex) > lastIndex ) error_overrun = true; // overrun
				break;
			default:
				error_instruction = true; // non-existent instruction
		}

		if ( currentIndex == lastIndex )
		{
			completed = true;
		}
		else
		{
			currentIndex += offset;
		}
		++step_count;
	}


	if ( completed )
	{
		std::cout << "Program completed successfuly, requiring " << step_count << " operations." << std::endl;
	}

	std::cout << "Accumulator was " << accumulator << std::endl;
	return completed;
}

/*
For part two, then it'd be all about looking at the most recent jmp or nop before you hit a repeated instruction, right?
I don't want to brute force, but that would work.
so... a stack of jmp or nop indexes?


*/

void day_08_problem_01()
{
	std::ifstream   infile;

	if ( !begin_problem( 8, 1, infile ) )
		return;


	Program prog = load_program( infile );

//	debug_dump_program( prog );

	run_program( prog );

}



void test_run_program()
{
	std::istringstream iss;
	Program prog;

	iss.str( day_08_test_complete_prog );

	prog = load_program( iss );

	std::cout << "This program should complete correctly with an acc value of 8." << std::endl;

	run_program( prog );
//---------
	iss.str( day_08_test_loop_prog );

	prog = load_program( iss );

	debug_dump_program( prog );

	std::cout << "This program would loop forever, and should terminate with an acc value of 5." << std::endl;

	run_program( prog );


}


