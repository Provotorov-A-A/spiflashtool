//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <iostream>
#include <algorithm>

#include "core/include/arg_parser.h"
#include "core/include/serial.h"
#include "core/include/progdevice.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;

enum ToolOperationType
{
	TOOL_OPERATION_NOT_SPECIFIED = 0,
	TOOL_OPERATION_READ_MEM,
	TOOL_OPERATION_WRITE_MEM,
	TOOL_OPERATION_READ_JEDEC_ID,
	TOOL_OPERATION_ERASE_MEM,
	TOOL_OPERATION_HELP_PRINT
};

class GlobalOptions
{
public:
	SerialOptions 				hardware_if_opts;
	ProgDeviceOptions			progdev_opts;
	bool 						print_help;	
	bool 						verbose;	

	GlobalOptions() : hardware_if_opts(), progdev_opts(), print_help(false), verbose(false)
	{};	
	
	void print() const
	{
		cout << "print_help  :" << print_help << endl;
		cout << "verbose     :" << verbose << endl;
		cout << "Hardware interface options:" << endl;
		hardware_if_opts.print();
		cout << "Programming device options:" << endl;
		progdev_opts.print();
	};
}; 

int main(int argc, char *argv[])
{
	GlobalOptions global_opts;
	cout << "==================================================================" << endl;
	global_opts.print();
	cout << "==================================================================" << endl;
	
	try
	{
		parse(argc, argv);
	}
	catch (ArgParserException& e)
	{
		((CommonException&)e).print();
	}
	catch(...)
	{
		cout << "UNKNOWN EXCEPTION RAISED!" << std::endl;
	}
	
	return 0;
}