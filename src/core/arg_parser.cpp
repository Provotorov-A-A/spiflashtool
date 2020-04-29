//******************************************************************************
//								INCLUDES
//******************************************************************************
#include "include/arg_parser.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;

std::vector<KeyValuePair>* get_command_key_pairs(size_t argc, char *argv[]);

//******************************************************************************
//								PROCEDURES	
//******************************************************************************
//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void parse(size_t argc, char *argv[])
{
	std::vector<KeyValuePair>* opt_pairs_list = get_command_key_pairs(argc, argv);
	
	cout << "Size = " << opt_pairs_list->size() << endl;
	for (size_t i = 0; i < opt_pairs_list->size(); ++i)
	{
		cout << "Pair[" << i << "] = " << (*opt_pairs_list)[i].key << " : " << (*opt_pairs_list)[i].value << endl;
	}
	
	for (size_t i = 0; i < opt_pairs_list->size(); ++i)
	{
		//----------------------------------------------------------------------
		// ProgDevice options
		// -read
		if (0 ==  (*opt_pairs_list)[i].key.compare("-read") )
		{
			cout << "Read is parsed!" << endl;
			continue;
		}
		
		//----------------------------------------------------------------------
		// -write
		if (0 == (*opt_pairs_list)[i].key.compare("-write") )
		{
			cout << "Read is parsed!" << endl;
			continue;
		}
		
		//----------------------------------------------------------------------
		// Hardware
		
		//----------------------------------------------------------------------
		// Unknown key
		throw(ArgParserException("Unknown key"));
		return;
	
	}
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
std::vector<KeyValuePair>* get_command_key_pairs(size_t argc, char *argv[])
{
	std::vector<KeyValuePair> *pairsList;
	string key_str;
	string val_str;
	bool wait_value = false;
	bool token_is_value;
	string empty_str = string("");
	
	// If first token is a value - error
	if ('-' != argv[1][0])
	{
		throw(ArgParserException("Argument parsing error: expected key, but value founded"));
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
				string msg = "Arg[" + std::to_string(i) + "] error: invalid key \"" + string(argv[i]) + "\"";
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