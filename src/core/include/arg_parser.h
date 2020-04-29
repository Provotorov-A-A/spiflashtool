#ifndef  _ARGPARSER_H_
#define _ARGPARSER_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <string>
#include <vector>
#include <iostream>
#include "common_exception.h"

//******************************************************************************
//								TYPES
//******************************************************************************
//==============================================================================
struct KeyValuePair
{
	std::string key;
	std::string value;
};

//==============================================================================
class ArgParserException : CommonException
{
public:
	ArgParserException(const std::string& str) : CommonException(std::string("Argument parser"), str) {};
};

//******************************************************************************
//								PROCEDURES	
//******************************************************************************
void parse(size_t argc, char *argv[]);
std::vector<KeyValuePair>* get_command_key_pairs(size_t argc, char *argv[]);
#endif /* _ARGPARSER_H_ */