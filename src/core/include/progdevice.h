#ifndef  _PROGDEVICE_H_
#define _PROGDEVICE_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <cstdint>
#include <string>

#include "common_exception.h"

//******************************************************************************
//								TYPES
//******************************************************************************

enum ProgDevOperationType
{
	PROGDEV_OPERATION_NOT_SPECIFIED = 0,
	PROGDEV_OPERATION_READ_MEM,
	PROGDEV_OPERATION_WRITE_MEM,
	PROGDEV_OPERATION_READ_JEDEC_ID,
	PROGDEV_OPERATION_ERASE_MEM,
};

static const std::string operation_enum_to_string[] = {"NOT_SPECIFIED", "READ_MEM", "WRITE_MEM", "READ_JEDEC_ID", "ERASE_MEM"};

struct ProgDeviceOptions
{
	size_t 					address;
	size_t 					size;
	std::string 			filename;
	bool 					check_crc;
	bool 					verbose;	

	ProgDeviceOptions() : 	address(0), size(0), filename(""), 
							check_crc(false), verbose(false)
							{ };
	void print() const
	{
		std::cout << "	address 	: " << address << std::endl;
		std::cout << "	size 		: " << size << std::endl;
		std::cout << "	filename 	: " << filename << std::endl;
		std::cout << "	check_crc 	: " << check_crc << std::endl;
		std::cout << "	verbose 	: " << verbose << std::endl;
	};
};


#endif /* _PROGDEVICE_H_ */