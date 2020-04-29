#ifndef  _GLOBAL_OPTS_H_
#define _GLOBAL_OPTS_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <iostream>
#include <string>

#include "serial.h"
#include "progdevice.h"
#include "common_exception.h"

//******************************************************************************
//								TYPES
//******************************************************************************
enum ToolOperationType
{
	TOOL_OPERATION_NOT_SPECIFIED = 0,
	TOOL_OPERATION_READ_MEM,
	TOOL_OPERATION_WRITE_MEM,
	TOOL_OPERATION_READ_JEDEC_ID,
	TOOL_OPERATION_ERASE_MEM,
	TOOL_OPERATION_HELP_PRINT,
	
	TOOL_OPERATION_TOTAL_NUM
};

class GlobalOptions
{
private:
	static const std::string operation_enum_to_string[];
public:
	SerialOptions 				hardware_if_opts;
	ProgDeviceOptions			progdev_opts;
	ToolOperationType			operation;
	bool 						print_help;	
	bool 						verbose;	

	GlobalOptions() : hardware_if_opts(), progdev_opts(), print_help(false), verbose(false)
	{};	
	
	void print() const;
}; 


#endif /* _GLOBAL_OPTS_H_ */