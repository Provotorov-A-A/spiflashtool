//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <stdio.h>
#include <vector>
#include <exception>

#include "core/include/arg_parser.h"
#include "core/include/application.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;

enum ApplicationResult : int
{
	APP_RESULT_FAIL = -1,
	APP_RESULT_OK = 0
};

//******************************************************************************
//								VARIABLES
//******************************************************************************

//******************************************************************************
//								PROCEDURES
//******************************************************************************

//******************************************************************************
//								ENTRY
//******************************************************************************
int main(int argc, char *argv[])
{
	ApplicationResult result = APP_RESULT_FAIL;
	std::vector<ArgParser::KeyValuePair>* arg_list;
	std::cout.flush();
	std::cerr.flush();
	
	try
	{
		arg_list = ArgParser::get_command_key_pairs(argc, argv);
		SpiflashtoolApplication application(arg_list);
		application.run();
		result = APP_RESULT_OK;
	}
	catch (ArgParser::ArgParserException& arg_err)
	{
		std::cerr.flush();
		std::cerr << "Command line syntax error in argument[" << arg_err.get_arg_num() << "] \"" << arg_err.get_arg_value() << "\"" << std::endl;
	}
	catch (ApplicationError& app_err)
	{
		std::cerr.flush();
		std::cerr << app_err.what() << std::endl;
	}
	catch (...)
	{
		std::cerr.flush();
		std::cerr << "Unexpected error ocurred"<< std::endl;
	}
	return result;
}