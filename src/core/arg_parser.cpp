//******************************************************************************
//								INCLUDES
//******************************************************************************
#include "include/arg_parser.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;
using namespace ArgParser;

//******************************************************************************
//								VARIABLES
//******************************************************************************
const std::string ArgParser::ArgParserException::module = "Argument parser";

//******************************************************************************
//								PROCEDURES	
//******************************************************************************
//------------------------------------------------------------------------------
// Allocates and returns vector with parsed pairs key-value. 
// Checks every key has no or has one value token.
// Throws: ArgParserException
//------------------------------------------------------------------------------
std::vector<KeyValuePair>* ArgParser::get_command_key_pairs(size_t argc, char *argv[])
{
	std::vector<KeyValuePair> *pairsList;
	string key_str;
	string val_str;
	bool wait_value = false;
	bool token_is_value;
	string empty_str = string("");
	
	if (argc < 2)
	{
		throw(ArgParserException("no arguments"));
	}
	
	pairsList = new std::vector<KeyValuePair>;
	
	for (size_t i = 1; i < argc; ++i)
	{
		token_is_value = ('-' != argv[i][0]);
		
		if (wait_value)
		{	
			if (token_is_value)
			{
				// Wait value, receive value
				// Save key-value pair
				val_str = std::string(argv[i]);
				pairsList->push_back({key_str, val_str});
				wait_value = false;
			}
			else				
			{
				// Wait value, receive key 
				pairsList->push_back({key_str, empty_str});
				
				key_str = std::string(argv[i]);
				
				// if this is last token
				if ((argc-1) == i)
				{
					pairsList->push_back({key_str, empty_str});
					break;
				}
				wait_value = true;
			}
		}
		else
		{
			if (token_is_value)
			{
				// Wait key, receive value
				// Error
				string msg = "Expect key at Arg[" + std::to_string(i) + "] but value \"" + string(argv[i]) + "\" found";
				pairsList->clear();
				delete pairsList;
				throw(ArgParserException(msg));
			}
			else
			{
				// Wait key, receive key
				key_str = std::string(argv[i]);
				
				// if this is last token
				if ((argc-1) == i)
				{
					pairsList->push_back({key_str, empty_str});
					break;
				}
				wait_value = true;
			}
		}
	}
	return pairsList;
}