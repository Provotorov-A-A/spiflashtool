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
namespace ArgParser
{
	//--------------------------------------------------------------------------
	struct KeyValuePair
	{
		std::string key;
		std::string value;
	};
	
	//--------------------------------------------------------------------------
	class ArgParserException 
	{
	private:
		unsigned int error_argument_num;
		std::string error_arg;
	public:
		ArgParserException(const unsigned int error_arg_num, const std::string& err_arg) : error_argument_num(error_arg_num), error_arg(err_arg) {};
		unsigned int get_arg_num() {return error_argument_num;};
		const char* get_arg_value() {return error_arg.c_str();};
	};
	
	//--------------------------------------------------------------------------
	std::vector<KeyValuePair>* get_command_key_pairs(size_t argc, char *argv[]);
}

//******************************************************************************
#endif /* _ARGPARSER_H_ */