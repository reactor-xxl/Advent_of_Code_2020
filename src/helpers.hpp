#pragma once
#include <vector>
#include <string>
#include <fstream>

bool begin_problem( int day, int part, std::ifstream& infile );
std::vector<std::string>	string_split( const std::string& str, char delim );
std::vector<std::string>	string_split_ignore_empty( const std::string& str, char delim );
