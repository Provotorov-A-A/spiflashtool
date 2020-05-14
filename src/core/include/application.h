#ifndef  _APPLICATION_H_
#define _APPLICATION_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#ifdef _TARGET_WINDOWS_
#include "../../target_specific/windows/win_serial.h"
#else
#ifdef 	_TARGET_UNIX_
#include "../../target_specific/unix/unix_serial.h"
#else
#warning "No OS specific serial header specified! Use _TARGET_WINDOWS_ or _TARGET_UNIX_ macros."
#endif
#endif

#include "common_exception.h"
#include "arg_parser.h"

#include "interfaces/serial_interface.h"
#include "interfaces/prog_device_interface.h"
#include "interfaces/base_spi_mem_device_interface.h"


#include "prog_device_impl.h"
#include "base_spi_mem_device_impl.h"
#include "base_spi_mem_driver_impl.h"

//******************************************************************************
//								TYPES
//******************************************************************************
struct GlobalOptions;
class ApplicationError;
class SpiflashtoolApplication;

//==============================================================================
/* Struct contained all the application's parameters*/
//==============================================================================
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
	std::string 				mem_id;	
	ToolOperationType			operation;
	size_t 						address;
	size_t 						size;
	std::string 				filename;	
	bool print_help;	
	bool verbose;	
	SerialOptions 				hardware_if_opts;
	ProgDeviceOptions			progdev_opts;
	
	//--------------------------------------------------------------------------
	GlobalOptions() : 	mem_id(""), operation(TOOL_OPERATION_NOT_SPECIFIED), 
						address(0), size(0), filename(""), print_help(false), 
						verbose(false), hardware_if_opts(), progdev_opts()
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
		std::cout << "memory id   : " << mem_id << std::endl;
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
//==============================================================================
class SpiflashtoolApplication
{
private:
	static const std::string app_name;
	SerialInterface* serial;
	ProgDeviceInterface* program_device;
	
	BaseSpiMemoryDeviceInterface* mem_dev;
	BaseSpiMemoryDriverInterface* mem_dev_driver;
	
	static const std::string help_str;
	GlobalOptions global_opts;

	void selectMemory(const std::string& mem_name_token, ProgDeviceInterface* const pd);
	void parse(std::vector<ArgParser::KeyValuePair>* pArgsList);
	void help_print() const ;
	bool is_hardware_operation(const GlobalOptions::ToolOperationType op) const;
	bool key_is_operation(const std::string&) const;
public:
	//--------------------------------------------------------------------------
	SpiflashtoolApplication(std::vector<ArgParser::KeyValuePair>* pArgsList);
	const char* get_name() const {return app_name.c_str();} ;
	//--------------------------------------------------------------------------	
	void run();
	~SpiflashtoolApplication()
	{
		if (mem_dev_driver) delete mem_dev_driver;
		if (mem_dev) 		delete mem_dev;
		if (program_device) delete program_device;
		if (serial) 		delete serial;
	}
};

//==============================================================================
// Application exception class
//==============================================================================
class ApplicationError
{
private:
	const SpiflashtoolApplication* application;
	std::string description;
	
public:
	ApplicationError (const SpiflashtoolApplication* app, const std::string& str) : application(app), description(str)
	{
		
	};
	const char* what() 
	{
		return ((std::string)(application->get_name()) + (std::string)" application exception occured: " + (std::string)(description.c_str())).c_str();
	};
};

//******************************************************************************

#endif /* _APPLICATION_H_ */