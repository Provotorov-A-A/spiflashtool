#ifndef  _APPLICATION_H_
#define _APPLICATION_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "common_exception.h"
#include "arg_parser.h"
#include "serial_interface.h"
#include "progdevice.h"
#include "include/spiflash_mem_driver_base.h"
#include "include/base_spi_mem_dev.h"
#include "../../target_specific/include/win_serial.h"

//******************************************************************************
//								TYPES
//******************************************************************************
//==============================================================================
/* Struct contained all the application's parameters*/
struct GlobalOptions
{
public:
	enum ToolOperationType
	{
		TOOL_OPERATION_NOT_SPECIFIED = 0,
		TOOL_OPERATION_READ_MEM,
		TOOL_OPERATION_WRITE_MEM,
		TOOL_OPERATION_READ_JEDEC_ID,
		TOOL_OPERATION_ERASE_MEM,
		TOOL_OPERATION_HELP_PRINT,
		TOOL_OPERATION_JUMP,
		TOOL_OPERATION_LINK,
		
		TOOL_OPERATION_TOTAL_NUM
	};
	
private:
	std::string operation_enum_to_string[ToolOperationType::TOOL_OPERATION_TOTAL_NUM];

public:		
	ToolOperationType			operation;
	size_t 						address;
	size_t 						size;
	std::string 				filename;	
	bool print_help;	
	bool verbose;	
	SerialOptions 				hardware_if_opts;
	ProgDeviceOptions			progdev_opts;
	
	//--------------------------------------------------------------------------
	GlobalOptions() : 	operation(TOOL_OPERATION_NOT_SPECIFIED), address(0), size(0), 
						filename(""), print_help(false), verbose(false), 
						hardware_if_opts(), progdev_opts()
	{
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_NOT_SPECIFIED] 	= "Not specified";
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_READ_MEM] 		= "Read memory";
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_WRITE_MEM] 		= "Program memory";
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_READ_JEDEC_ID] 	= "Read JEDEC ID";
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_ERASE_MEM] 		= "Erase memory";
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_HELP_PRINT] 		= "Print help";
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_JUMP] 			= "Jump to address";
		GlobalOptions::operation_enum_to_string[ToolOperationType::TOOL_OPERATION_LINK] 			= "Test programming device connect";
	};	
	//--------------------------------------------------------------------------
	void print() const
	{
		std::cout << "operation   : " << GlobalOptions::operation_enum_to_string[operation] << std::endl;
		std::cout << "address 	  : " << std::hex << std::showbase << address << std::endl;
		std::cout << "size 		  : " << std::setbase(10) << size << std::endl;
		std::cout << "filename 	  : " << filename << std::endl;	
		std::cout << "print_help  : " << print_help << std::endl;
		std::cout << "verbose     : " << verbose << std::endl;
		std::cout << "Hardware interface options:" << std::endl;
		hardware_if_opts.print();
		std::cout << "Programming device options:" << std::endl;
		progdev_opts.print();		
	};
}; 

//==============================================================================
/* Class that implements main algorithm */
class SpiflashtoolApplication
{
private:
	static const std::string help_str;
	GlobalOptions global_opts;
	
	void parse(std::vector<ArgParser::KeyValuePair>* pArgsList);
	
public:
	//--------------------------------------------------------------------------
	SpiflashtoolApplication(std::vector<ArgParser::KeyValuePair>* pArgsList)
	{
		try 
		{
			parse(pArgsList);
		}
		catch	(const ArgParser::ArgParserException& e)
		{
			throw e;
		}
	};
	//--------------------------------------------------------------------------	
	void run();
	void help_print() const ;
};

//******************************************************************************

#endif /* _APPLICATION_H_ */