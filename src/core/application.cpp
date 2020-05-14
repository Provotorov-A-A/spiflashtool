//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <iomanip>

#include "include/application.h"

using namespace std;

//******************************************************************************
//								TYPES
//******************************************************************************
//==============================================================================
// Exception class for application arguments parsing
//==============================================================================
class ApplicationParsingError
{
	std::string msg;
	std::string err_key;
	std::string err_val;
public:
	ApplicationParsingError(const std::string& msg, const std::string& key, const std::string& value) : msg(msg), err_key(key), err_val(value) {};
	const char* get_msg(){return msg.c_str();};
	const char* get_key(){return err_key.c_str();};
	const char* get_value(){return err_val.c_str();};
};

//******************************************************************************
//								VARIABLES
//******************************************************************************
const std::string SpiflashtoolApplication::app_name = "spiflashtool";

//******************************************************************************
//								PROCEDURES
//******************************************************************************
extern size_t str_to_uint(const std::string s);		// utility function

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
SpiflashtoolApplication::SpiflashtoolApplication(std::vector<ArgParser::KeyValuePair>* pArgsList)
{
	static int instances = 0;
	if (instances !=0)
	{
		throw ApplicationError(this, "try to create second application class instance");
		return;
	}
	else
	{
		instances++;
	}
	
	try 
	{
		parse(pArgsList);
		if (is_hardware_operation(global_opts.operation))
		{
			serial = new WindowsSerial(global_opts.hardware_if_opts);
			program_device = new ProgDevice(serial);
			selectMemory(global_opts.mem_id, program_device);
		}				
	}
	catch (ApplicationParsingError& ex)
	{
		if (0 != pArgsList)
		{
			pArgsList->clear();
			delete pArgsList;
		}
		std::string msg = (std::string)"Error \"" + (std::string)ex.get_msg() + (std::string)"\" occured with key \"" + (std::string)ex.get_key() + (std::string)"\" and value \"" + (std::string)ex.get_value() + (std::string)"\"\n";

		throw(ApplicationError(this, msg));
	}
	catch (const CommonException& e)
	{
		throw ApplicationError(this, e.what()); 
	}
};

//------------------------------------------------------------------------------
// Choose active memory device and memory driver for specified option
//------------------------------------------------------------------------------
void SpiflashtoolApplication::selectMemory(const std::string& mem_name_token, ProgDeviceInterface* const pd)
{
	if ( (0 == mem_name_token.compare("")) || (0 == mem_name_token.compare("base")) )
	{
		mem_dev = new BaseSpiMemoryDevice(pd);
		mem_dev_driver = new BaseSpiMemoryDriver(mem_dev);
	}
	else
	{
		throw std::runtime_error("invalid memory identificator was specified");
	}
}

//------------------------------------------------------------------------------
// Prints help string
//------------------------------------------------------------------------------
void SpiflashtoolApplication::help_print() const
{
	static const std::string help_str = std::string(
		"Usage: spiflashtool [operation] <key1> <value1> <key2> <value2> ...\n" \
		"operation: -read, -write, -read_id, -erase, -help\n" \
		"Keys:\n" \
		"  -m              selects memory and memory driver to work with. If not specified 'base' configuration will be performed (only base SPI Flash operations)\n" \
		"  -address        start address in flash memory for specified operation. Default value = 0.\n" \
		"  -size           data size in bytes that will be read/write/erased. Default value = 0. For write operation 0 means \"all file\"\n" \
		"  -filename       data source for write operation and data destination for read operation\n" \
		"  -crc            switch on CRC16 calculating for every data packet\n" \
		"  -verbose        full process listing\n" \
		"  -port           serial port name\n" \
		"  -baudrate       serial baudrate\n"
	);
	
	std::cout << help_str << std::endl;
}

//------------------------------------------------------------------------------
// Main application algorithm
//------------------------------------------------------------------------------
void SpiflashtoolApplication::run()
{
	if (global_opts.verbose)
	{
		std::cout << std::endl;
		std::cout << "Program configuration: " << std::endl;
		global_opts.print();
		std::cout << std::endl;
	}

	try 
	{


		// Perform specified operation
		switch (global_opts.operation)
		{
			case GlobalOptions::ToolOperationType::TOOL_OPERATION_HELP_PRINT:
			{
				help_print();
			}
			break;

			case GlobalOptions::ToolOperationType::TOOL_OPERATION_LINK:
			{
				if (global_opts.verbose) std::cout << "Check programming device connection ..." << std::endl;
				program_device->util_link();
				if (global_opts.verbose) std::cout << "... Connection OK" << std::endl;
			}
			break;

			case GlobalOptions::ToolOperationType::TOOL_OPERATION_JUMP:
			{
				uint32_t address = global_opts.address;
				if (global_opts.verbose) std::cout << "Jump to address " << std::hex << std::showbase << address << std::endl;
				program_device->util_exit_to(address);
				if (global_opts.verbose) std::cout << "... Jump to address OK" << std::endl;
			}
			break;
			
			case GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_JEDEC_ID:
			{
				if (global_opts.verbose) std::cout << "Read id ..." << std::endl;
				std::cout.flush();
				uint32_t id = mem_dev_driver->read_id();
				std::cout << "Read id OK, id = " << std::noshowbase << std::hex << std::setw(2) << (int)(id & 0xFF) << " " << (int)((id >> 8)&0xFF) << " " << (int)((id >> 16)&0xFF) << std::endl;
			}
			break;

			case GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_MEM:
			{
				uint32_t address = global_opts.address;
				size_t size = global_opts.size;
				std::cout << std::setbase(10);
				
				mem_dev_driver->read_mem_to_file(global_opts.filename, address, size);
				if (global_opts.verbose) std::cout << "... Read data OK" << std::endl;
			}
			break;

			case GlobalOptions::ToolOperationType::TOOL_OPERATION_ERASE_MEM:
			{
				uint32_t address = global_opts.address;
				size_t size = global_opts.size;
				mem_dev_driver->erase(address, size);
			}
			break;
			
			case GlobalOptions::ToolOperationType::TOOL_OPERATION_WRITE_MEM:
			{
				uint32_t address = global_opts.address;
				size_t size = global_opts.size;

				mem_dev_driver->write_mem_from_file(global_opts.filename, address, size);
				if (global_opts.verbose) std::cout << "... Program data OK" << std::endl;
			}
			break;

			default:
			{
				if (global_opts.verbose) std::cout << "No operation was specified. Exit." << std::endl;
			}
			break;
		}
	}
	catch (const CommonException& e)
	{
		throw ApplicationError(this, e.what()); 
	}
}

//------------------------------------------------------------------------------
// Parse command line. Configure all submodules with parsed options.
// All new options handlers must be defined here.
// Throws: ArgParser::ArgParserException
//------------------------------------------------------------------------------
void SpiflashtoolApplication::parse(std::vector<ArgParser::KeyValuePair>* opt_pairs_list)
{
	bool operation_is_defined = false;
	
	for (size_t i = 0; i < opt_pairs_list->size(); ++i)
	{
		// It's illegal to specify more than one operation token. Check it.
		if (key_is_operation((*opt_pairs_list)[i].key))
		{
			if (operation_is_defined)
			{
				throw ApplicationParsingError("duplicated operation key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			else
			{
				operation_is_defined = true;
			}
		}
		
// -read
		if (0 == (*opt_pairs_list)[i].key.compare("-read") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_MEM;
			continue;
		}
// -write
		if (0 == (*opt_pairs_list)[i].key.compare("-write") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_WRITE_MEM;
			continue;
		}
// -read_id
		if (0 == (*opt_pairs_list)[i].key.compare("-read_id") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_JEDEC_ID;
			continue;
		}
// -erase
		if (0 == (*opt_pairs_list)[i].key.compare("-erase") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_ERASE_MEM;
			continue;
		}

// -link		
		if (0 == (*opt_pairs_list)[i].key.compare("-link") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_LINK;
			continue;
		}		

// -jump
		if (0 == (*opt_pairs_list)[i].key.compare("-jump") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_JUMP;
			continue;
		}		
		
// -help
		if ( (0 == (*opt_pairs_list)[i].key.compare("-help") ) || (0 == (*opt_pairs_list)[i].key.compare("--help")) )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_HELP_PRINT;
			continue;
		}
// -m
		if (0 == (*opt_pairs_list)[i].key.compare("-m") )
		{
			if (0 == (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.mem_id = (*opt_pairs_list)[i].value;
			continue;
		}
		
// -address
		if (0 == (*opt_pairs_list)[i].key.compare("-address") )
		{
			if (0 == (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			size_t address;
			address = str_to_uint((*opt_pairs_list)[i].value);
			global_opts.address = address;
			continue;
		}
// -size
		if (0 == (*opt_pairs_list)[i].key.compare("-size") )
		{
			if (0 == (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}			
			size_t size;
			size = str_to_uint((*opt_pairs_list)[i].value);
			global_opts.size = size;
			continue;
		}
// -filename
		if (0 == (*opt_pairs_list)[i].key.compare("-filename") )
		{
			if (0 == (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}			
			global_opts.filename = (*opt_pairs_list)[i].value;
			continue;
		}
// -crc
		if (0 == (*opt_pairs_list)[i].key.compare("-crc") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.progdev_opts.check_crc = true;
			continue;
		}
// -verbose
		if (0 == (*opt_pairs_list)[i].key.compare("-verbose") )
		{
			if (0 != (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}
			global_opts.verbose = true;
			continue;
		}
// -port
		if (0 == (*opt_pairs_list)[i].key.compare("-port") )
		{
			if (0 == (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}			
			global_opts.hardware_if_opts.name = (*opt_pairs_list)[i].value;
			continue;
		}
// -baudrate
		if (0 == (*opt_pairs_list)[i].key.compare("-baudrate") )
		{
			if (0 == (*opt_pairs_list)[i].value.compare("") )
			{
				throw ApplicationParsingError("invalid value for specified key", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
			}			
			size_t baudrate;
			baudrate = str_to_uint((*opt_pairs_list)[i].value);
			global_opts.hardware_if_opts.baudrate = baudrate;
			continue;
		}
		//----------------------------------------------------------------------
		// Unknown key
		//std::cerr << "Unknown key was found" << std::endl;
		throw ApplicationParsingError("Unknown key was found", (*opt_pairs_list)[i].key, (*opt_pairs_list)[i].value);
		return;			
	}	/*for ...*/
}

//------------------------------------------------------------------------------
// Returns true if token is operation command
//------------------------------------------------------------------------------
bool SpiflashtoolApplication::key_is_operation(const std::string& s) const
{
	if (0 == s.compare("-read")) return true;
	if (0 == s.compare("-write")) return true;
	if (0 == s.compare("-read_id")) return true;
	if (0 == s.compare("-erase")) return true;
	if (0 == s.compare("-link")) return true;
	if (0 == s.compare("-jump")) return true;
	if ( (0 == s.compare("-help")) || (0 == s.compare("--help")) ) return true;
	
	return false;
}

//------------------------------------------------------------------------------
// Returns true if hardware initialization required for specified operation  
//------------------------------------------------------------------------------
bool SpiflashtoolApplication::is_hardware_operation(const GlobalOptions::ToolOperationType op) const
{
	if (GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_MEM == op) return true;
	if (GlobalOptions::ToolOperationType::TOOL_OPERATION_WRITE_MEM == op) return true;
	if (GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_JEDEC_ID == op) return true;
	if (GlobalOptions::ToolOperationType::TOOL_OPERATION_ERASE_MEM == op) return true;
	if (GlobalOptions::ToolOperationType::TOOL_OPERATION_LINK == op) return true;
	if (GlobalOptions::ToolOperationType::TOOL_OPERATION_JUMP == op) return true;
	
	return false;
}
