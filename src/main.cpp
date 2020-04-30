//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <iostream>
#include <algorithm>

#include "core/include/arg_parser.h"
#include "core/include/serial_interface.h"
#include "core/include/progdevice.h"
#include "core/include/global_opts.h"
#include "target_specific/include/win_serial.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;

//******************************************************************************
//								VARIABLES
//******************************************************************************
GlobalOptions global_opts;

static const std::string help_str= std::string
(
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
static void help_print()
{
	std::cout << std::endl << help_str;
}

//******************************************************************************
//								ENTRY
//******************************************************************************
int main(int argc, char *argv[])
{
	// Command line arguments parsing
	try
	{
		parse(argc, argv);
	}
	catch (ArgParserException& e)
	{
		((CommonException&)e).print();
		exit(-1);
	}
	catch(...)
	{
		std::cout << "UNKNOWN EXCEPTION RAISED!" << std::endl;
		exit(-1);
	}

	cout << "==================================================================" << endl;
	global_opts.print();
	cout << "==================================================================" << endl;


//----------------------------------------------
// DEBUG
	try 
	{
		WindowsSerial serial(global_opts.hardware_if_opts);
		ProgDevice pd((SerialInterface*)&serial);
		pd.util_link();
	}
	catch (ArgParserException& e)
	{
		((CommonException&)e).print();
	}
	catch (HW_IO_Error& e)
	{
		((CommonException&)e).print();
	}
	catch (ProgDevice_Error& e)
	{
		((CommonException&)e).print();
	}	
	catch(...)
	{
		std::cout << "UNKNOWN EXCEPTION RAISED!" << std::endl;
	}	
// DEBUG END
//----------------------------------------------


	// Perform specified operation
	switch (global_opts.operation)
	{
		case TOOL_OPERATION_HELP_PRINT:
		{
			help_print();
		}
		break;

		case TOOL_OPERATION_READ_JEDEC_ID:
		{
			
		}
		break;

		
		default:
		{
			std::cout << "No operation was specified. Exit." << std::endl;
		}
	}

		
	return 0;
}