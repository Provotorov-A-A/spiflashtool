#ifndef  _SERIAL_H_
#define _SERIAL_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <cstdint>
#include <string>

#include "common_exception.h"

//******************************************************************************
//								TYPES
//******************************************************************************

enum SerialOptionsParity
{
	SERIAL_OPTIONS_PARITY_NOPARITY = 0,
	SERIAL_OPTIONS_PARITY_ODDPARITY = 1,
	SERIAL_OPTIONS_PARITY_EVENPARITY = 2,
	SERIAL_OPTIONS_PARITY_MARKPARITY = 3,
	SERIAL_OPTIONS_PARITY_SPACEPARITY = 4,
	
	SERIAL_OPTIONS_PARITY_TOTAL_NUM
};

//------------------------------------------------------------------------------
enum SerialOptionsStopBits
{
	SERIAL_OPTIONS_STOPBITS_ONESTOPBIT = 0,
	SERIAL_OPTIONS_STOPBITS_ONE5STOPBITS = 1,
	SERIAL_OPTIONS_STOPBITS_TWOSTOPBITS = 2,
	
	SERIAL_OPTIONS_STOPBITS_TOTAL_NUM
};

//******************************************************************************
//								VARIABLES
//******************************************************************************

static const std::string parity_enum_to_string[SERIAL_OPTIONS_PARITY_TOTAL_NUM] = {"NOPARITY", "ODDPARITY", "EVENPARITY", "MARKPARITY", "SPACEPARITY"};
static const std::string stopbits_enum_to_string[SERIAL_OPTIONS_STOPBITS_TOTAL_NUM] = {"ONESTOPBIT", "ONE5STOPBITS", "TWOSTOPBITS"};

//******************************************************************************
//								TYPES
//******************************************************************************

//------------------------------------------------------------------------------
struct SerialOptions
{
	std::string				name;
	uint32_t 				baudrate;
	SerialOptionsParity		parity;			// 0 - NOPARITY, 	1 - ODDPARITY, 		2 - EVENPARITY, 	3 - MARKPARITY, 	4 - SPACEPARITY
	SerialOptionsStopBits	stopbits;		// 0 - ONESTOPBIT, 	1 - ONE5STOPBITS, 	2 - TWOSTOPBITS

	SerialOptions() : 		name(""), 
							baudrate(9600), 
							parity(SERIAL_OPTIONS_PARITY_NOPARITY),
							stopbits(SERIAL_OPTIONS_STOPBITS_ONESTOPBIT)
							
	{};
	
	void print() const
	{
		std::cout << "	name 		: " << name << std::endl;
		std::cout << "	baudrate 	: " << baudrate << std::endl;
		std::cout << "	parity 		: " << parity_enum_to_string[parity] << std::endl;
		std::cout << "	stopbits 	: " << stopbits_enum_to_string[stopbits] << std::endl;
	}
};


#endif /* _SERIAL_H_ */