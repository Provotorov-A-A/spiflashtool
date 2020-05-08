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
extern void printMem(const uint8_t* const pMemStart, const size_t size, const size_t offset);

//******************************************************************************
//								PROCEDURES
//******************************************************************************

//******************************************************************************
//								ENTRY
//******************************************************************************
int main(int argc, char *argv[])
{
	std::vector<ArgParser::KeyValuePair>* arg_list;
	
	try
	{
		arg_list = ArgParser::get_command_key_pairs(argc, argv);
		
		SpiflashtoolApplication application(arg_list);
		application.run();
	}
	catch (const ArgParser::ArgParserException& e)
	{
		e.print();
		if (arg_list) 
		{
			arg_list->clear();
			delete arg_list;
		}
		exit(ApplicationResult::APP_RESULT_FAIL);
	}
	catch (const CommonException& e)
	{
		e.print();
		exit(ApplicationResult::APP_RESULT_FAIL);
	}
	catch (const std::exception& e)
	{
		e.what();
		exit(ApplicationResult::APP_RESULT_FAIL);
	}
	
	return ApplicationResult::APP_RESULT_OK;
}


