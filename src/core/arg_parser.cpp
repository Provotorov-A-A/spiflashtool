//******************************************************************************
//								INCLUDES
//******************************************************************************
#include "include/arg_parser.h"
#include "include/global_opts.h"
#include "include/serial.h"
#include "include/progdevice.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;

static std::vector<KeyValuePair>* get_command_key_pairs(size_t argc, char *argv[]);
static size_t str_to_uint(std::string s);
//******************************************************************************
//								VARIABLES
//******************************************************************************
extern GlobalOptions global_opts;

//******************************************************************************
//								PROCEDURES	
//******************************************************************************
//------------------------------------------------------------------------------
// Parse command line. Configure all submodules with parsed options.
// All new options handlers must be defined here.
// Throws: ArgParserException
//------------------------------------------------------------------------------
void parse(size_t argc, char *argv[])
{
	std::vector<KeyValuePair>* opt_pairs_list = get_command_key_pairs(argc, argv);
	
	cout << "Size = " << opt_pairs_list->size() << endl;
	for (size_t i = 0; i < opt_pairs_list->size(); ++i)
	{
		cout << "Pair[" << i << "] = " << (*opt_pairs_list)[i].key << " : " << (*opt_pairs_list)[i].value << endl;
	}
	
	size_t cur_opt_index = 0;		// for exception handler
	try
	{
		for (size_t i = 0; i < opt_pairs_list->size(); ++i)
		{
			cur_opt_index = i;
	// -read
			if (0 == (*opt_pairs_list)[i].key.compare("-read") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument(""));
				}
				global_opts.operation = TOOL_OPERATION_READ_MEM;
				continue;
			}
	// -write
			if (0 == (*opt_pairs_list)[i].key.compare("-write") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument(""));
				}
				global_opts.operation = TOOL_OPERATION_WRITE_MEM;
				continue;
			}
	// -read_id
			if (0 == (*opt_pairs_list)[i].key.compare("-read_id") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument(""));
				}
				global_opts.operation = TOOL_OPERATION_READ_JEDEC_ID;
				continue;
			}
	// -erase
			if (0 == (*opt_pairs_list)[i].key.compare("-erase") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument(""));
				}
				global_opts.operation = TOOL_OPERATION_ERASE_MEM;
				continue;
			}
	// -help
			if (0 == (*opt_pairs_list)[i].key.compare("-help") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument(""));
				}
				global_opts.operation = TOOL_OPERATION_HELP_PRINT;
				continue;
			}
			
	// -address
			if (0 == (*opt_pairs_list)[i].key.compare("-address") )
			{
				size_t address;
				address = str_to_uint((*opt_pairs_list)[i].value);
				global_opts.progdev_opts.address = address;
				continue;
			}
	// -size
			if (0 == (*opt_pairs_list)[i].key.compare("-size") )
			{
				size_t size;
				size = str_to_uint((*opt_pairs_list)[i].value);
				global_opts.progdev_opts.size = size;
				continue;
			}
	// -filename
			if (0 == (*opt_pairs_list)[i].key.compare("-filename") )
			{
				global_opts.progdev_opts.filename = (*opt_pairs_list)[i].value;
				continue;
			}
	// -crc
			if (0 == (*opt_pairs_list)[i].key.compare("-crc") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument(""));
				}
				global_opts.progdev_opts.check_crc = true;
				continue;
			}
	// -verbose
			if (0 == (*opt_pairs_list)[i].key.compare("-verbose") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument(""));
				}
				global_opts.verbose = true;
				global_opts.progdev_opts.verbose = true;
				continue;
			}
	// -port
			if (0 == (*opt_pairs_list)[i].key.compare("-port") )
			{
				global_opts.hardware_if_opts.name = (*opt_pairs_list)[i].value;
				continue;
			}
	// -baudrate
			if (0 == (*opt_pairs_list)[i].key.compare("-baudrate") )
			{
				size_t baudrate;
				baudrate = str_to_uint((*opt_pairs_list)[i].value);
				global_opts.hardware_if_opts.baudrate = baudrate;
				continue;
			}
			
			//----------------------------------------------------------------------
			// Unknown key
			throw(ArgParserException("Unknown key" + (*opt_pairs_list)[i].key));
			return;			
		}	/*for ...*/
	}
	catch (ArgParserException& ex)
	{
		string msg = "Unknown key \"" + (*opt_pairs_list)[cur_opt_index].key + "\"";
		opt_pairs_list->clear();
		delete opt_pairs_list;
		throw(ArgParserException(msg));
	}
	catch (...)
	{
		string msg = "Value \"" + (*opt_pairs_list)[cur_opt_index].value + "\" is invalid for key \"" + (*opt_pairs_list)[cur_opt_index].key + "\"";
		opt_pairs_list->clear();
		delete opt_pairs_list;
		throw(ArgParserException(msg));
	}		
}

//------------------------------------------------------------------------------
// Convert string to unsigned integer
// Octal (prefix 0), decimal and hexical(prefix 0x or 0X) base are supported.
// Suffixes 'k', 'K', 'm', 'M', 'g', 'G' are supported
// Throws:
// std::invalid_argument, если преобразование не может быть выполнено
// std::out_of_range, если преобразованное значение будет выходить за границы диапазона значений типа результата
//------------------------------------------------------------------------------
static size_t str_to_uint(std::string s)
{
	string no_suffix;
	size_t mul = 1;
	size_t len = s.size();
	size_t res = 0;
	
	bool has_hex_prefix = (0 == s.substr(0,2).compare("0x")) || (0 == s.substr(0,2).compare("0X"));
	
	if ( (s.at(len-1) == 'k') || (s.at(len-1) == 'K') )
	{
		mul = 1024;
	}
	else if ( (s.at(len-1) == 'm') || (s.at(len-1) == 'M') )
	{
		mul = 1024*1024;
	}
	else if ( (s.at(len-1) == 'g') || (s.at(len-1) == 'G') )
	{
		mul = 1024*1024*1024;
	}
	else if ( has_hex_prefix && (!isxdigit(s.at(len-1))) )
	{
		throw(std::invalid_argument("bad prefix"));
	}
	
	if (1 != mul)
	{
		no_suffix = s.substr(0, len-1);
	}
	else
	{
		no_suffix = s;
	}
	
	size_t err_index = 0;
	res = stoull(no_suffix, &err_index, 0);
	if(err_index < no_suffix.size())
	{
		cout << "SECOND " << err_index << endl;
		throw(std::invalid_argument("bad prefix"));
	};
	return 	(res*mul);
	
}
//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
static std::vector<KeyValuePair>* get_command_key_pairs(size_t argc, char *argv[])
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