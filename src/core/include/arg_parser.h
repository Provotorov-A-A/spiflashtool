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
	class ArgParserException : public CommonException
	{
	private:
		static const std::string module;
	public:
		ArgParserException(const std::string& str) 	: CommonException(module, str) {};
		ArgParserException() 						: CommonException(module, "") {};
	};
	
	//--------------------------------------------------------------------------
	std::vector<KeyValuePair>* get_command_key_pairs(size_t argc, char *argv[]);
}

//******************************************************************************
#endif /* _ARGPARSER_H_ */