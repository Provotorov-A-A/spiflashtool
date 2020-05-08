//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <iomanip>

#include "include/application.h"

using namespace std;

//******************************************************************************
//								TYPES
//******************************************************************************

//******************************************************************************
//								VARIABLES
//******************************************************************************
const std::string SpiflashtoolApplication::help_str = std::string(
	"Usage: spiflashtool [operation] <key1> <value1> <key2> <value2> ...\n" \
	"operation: -read, -write, -read_id, -erase, -help\n" \
	"Keys:\n" \
	"  -address        start address in flash memory for specified operation. Default value = 0.\n" \
	"  -size           data size in bytes that will be read/write/erased. Default value = 0. For write operation 0 means \"all file\"\n" \
	"  -filename       data source for write operation and data destination for read operation\n" \
	"  -crc            switch on CRC16 calculating for every data packet\n" \
	"  -verbose        full process listing\n" \
	"  -port           serial port name\n" \
	"  -baudrate       serial baudrate\n"
);

//******************************************************************************
//								PROCEDURES
//******************************************************************************
extern size_t str_to_uint(const std::string s);

//------------------------------------------------------------------------------
// Starts application algorithm
//------------------------------------------------------------------------------
void SpiflashtoolApplication::help_print() const
{
	cout << help_str << endl;
}

//------------------------------------------------------------------------------
// Starts application algorithm
//------------------------------------------------------------------------------
void SpiflashtoolApplication::run()
{
	try 
	{
		WindowsSerial serial(global_opts.hardware_if_opts);
		ProgDevice program_device((SerialInterface*)&serial);
		BaseSpiMemoryDevice mem(BaseSpiMemoryDevice(&program_device, 1000000));
		BaseSpiFlashMemoryDriver mem_drv(&mem);

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
				std::cout << "Check programming device connection ..." << std::endl;
				program_device.util_link();
				std::cout << "... Connection OK" << std::endl;
			}
			break;

			case GlobalOptions::ToolOperationType::TOOL_OPERATION_JUMP:
			{
				uint32_t address = global_opts.address;
				std::cout << "Jump to address " << std::hex << std::showbase << address << std::endl;
				program_device.util_exit_to(address);
				std::cout << "... Jump to address OK" << std::endl;
			}
			break;
			
			case GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_JEDEC_ID:
			{
				std::cout << "Read id ..." << std::endl;
				std::cout.flush();
				uint32_t id = mem_drv.read_id();
				std::cout << "Read id OK, id = " << std::noshowbase << std::hex << std::setw(2) << (int)(id & 0xFF) << " " << (int)((id >> 8)&0xFF) << " " << (int)((id >> 16)&0xFF) << std::endl;
			}
			break;

			case GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_MEM:
			{
				uint32_t address = global_opts.address;
				size_t size = global_opts.size;
				std::cout << std::setbase(10);
				std::cout << "Read " << size << " byte(s) from address ";
				std::cout << std::hex << std::showbase;
				std::cout << address << " ..." << std::endl;
				
				mem_drv.read_mem_to_file(global_opts.filename, address, size);
				std::cout << "... Read data OK" << std::endl;
			}
			break;

			case GlobalOptions::ToolOperationType::TOOL_OPERATION_ERASE_MEM:
			{
				std::cout << "Erase data ... " << std::endl;
				uint32_t address = global_opts.address;
				size_t size = global_opts.size;
				mem_drv.erase(address, size);
				std::cout << "... Erase data OK" << std::endl;
			}
			break;
			
			case GlobalOptions::ToolOperationType::TOOL_OPERATION_WRITE_MEM:
			{
				uint32_t address = global_opts.address;
				size_t size = global_opts.size;

				std::cout << std::setbase(10);
				std::cout << "Program " << size << " byte(s) from address ";
				std::cout << std::hex << std::showbase;
				std::cout << address << " ..." << std::endl;

				mem_drv.write_mem_from_file(global_opts.filename, address, size);
				std::cout << "... Program data OK" << std::endl;
			}
			break;

			
			default:
			{
				std::cout << "No operation was specified. Exit." << std::endl;
			}
			break;
		}
	}
	catch (const CommonException& e)
	{
		std::cout << "... Operation FAILED" << std::endl;
		e.print();
		exit(-1);
	}
	catch(...)
	{
		std::cout << "Unknown exception rised!" << std::endl;
		exit(-1);
	}	
	
	std::cout << "FINISH" << std::endl;
}

//------------------------------------------------------------------------------
// Parse command line. Configure all submodules with parsed options.
// All new options handlers must be defined here.
// Throws: ArgParser::ArgParserException
//------------------------------------------------------------------------------
void SpiflashtoolApplication::parse(std::vector<ArgParser::KeyValuePair>* opt_pairs_list)
{
	bool operation_is_defined = false;
	size_t cur_opt_index = 0;			// for exception handler
	
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
					throw(std::invalid_argument("Extra value for " + (*opt_pairs_list)[i].key));
				}
				if (operation_is_defined)
				{
					throw(std::invalid_argument("duplicate operation key"));
				}
				global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_MEM;
				operation_is_defined = true;
				continue;
			}
	// -write
			if (0 == (*opt_pairs_list)[i].key.compare("-write") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument("Extra value for " + (*opt_pairs_list)[i].key));
				}
				if (operation_is_defined)
				{
					throw(std::invalid_argument("duplicate operation key"));
				}
				global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_WRITE_MEM;
				operation_is_defined = true;
				continue;
			}
	// -read_id
			if (0 == (*opt_pairs_list)[i].key.compare("-read_id") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument("Extra value for " + (*opt_pairs_list)[i].key));
				}
				if (operation_is_defined)
				{
					throw(std::invalid_argument("duplicate operation key"));
				}
				global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_READ_JEDEC_ID;
				operation_is_defined = true;
				continue;
			}
	// -erase
			if (0 == (*opt_pairs_list)[i].key.compare("-erase") )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument("Extra value for " + (*opt_pairs_list)[i].key));
				}
				if (operation_is_defined)
				{
					throw(std::invalid_argument("duplicate operation key"));
				}
				global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_ERASE_MEM;
				operation_is_defined = true;
				continue;
			}
	// -help
			if ( (0 == (*opt_pairs_list)[i].key.compare("-help") ) || (0 == (*opt_pairs_list)[i].key.compare("--help")) )
			{
				if (0 != (*opt_pairs_list)[i].value.compare("") )
				{
					throw(std::invalid_argument("Extra value for " + (*opt_pairs_list)[i].key));
				}
				if (operation_is_defined)
				{
					throw(std::invalid_argument("duplicate operation key"));
				}
				global_opts.operation = GlobalOptions::ToolOperationType::TOOL_OPERATION_HELP_PRINT;
				operation_is_defined = true;
				continue;
			}
			
	// -address
			if (0 == (*opt_pairs_list)[i].key.compare("-address") )
			{
				size_t address;
				address = str_to_uint((*opt_pairs_list)[i].value);
				global_opts.address = address;
				continue;
			}
	// -size
			if (0 == (*opt_pairs_list)[i].key.compare("-size") )
			{
				size_t size;
				size = str_to_uint((*opt_pairs_list)[i].value);
				global_opts.size = size;
				continue;
			}
	// -filename
			if (0 == (*opt_pairs_list)[i].key.compare("-filename") )
			{
				global_opts.filename = (*opt_pairs_list)[i].value;
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
				global_opts.verbose = true;
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
			throw(std::invalid_argument("Unknown key " + (*opt_pairs_list)[i].key));
			return;			
		}	/*for ...*/
	}
	//catch (std::invalid_argument& ex)
	catch (std::logic_error& ex)
	{
		string msg = "Parsing error with key \"" + (*opt_pairs_list)[cur_opt_index].key + "\" and value \"" + (*opt_pairs_list)[cur_opt_index].value + "\" : " + ex.what();
		opt_pairs_list->clear();
		delete opt_pairs_list;
		throw(ArgParser::ArgParserException(msg));
	}		
	catch (...)
	{
		string msg = "Parsing error with key \"" + (*opt_pairs_list)[cur_opt_index].key + "\" and value \"" + (*opt_pairs_list)[cur_opt_index].value + "\"";
	}	
}


