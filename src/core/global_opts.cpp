//******************************************************************************
//								INCLUDES
//******************************************************************************
#include "include/global_opts.h"

//******************************************************************************
//								TYPES
//******************************************************************************

//******************************************************************************
//								VARIABLES
//******************************************************************************
const std::string GlobalOptions::operation_enum_to_string[TOOL_OPERATION_TOTAL_NUM] = {"NOT_SPECIFIED", "READ_MEM", "WRITE_MEM", "JEDEC_ID", "ERASE_MEM", "HELP_PRINT"};

void GlobalOptions::print() const
{
	std::cout << "operation  : " << operation_enum_to_string[operation] << std::endl;
	std::cout << "print_help  : " << print_help << std::endl;
	std::cout << "verbose     : " << verbose << std::endl;
	std::cout << "Hardware interface options:" << std::endl;
	hardware_if_opts.print();
	std::cout << "Programming device options:" << std::endl;
	progdev_opts.print();
};