#ifndef _WIN_SERIAL_H_
#define _WIN_SERIAL_H_
//******************************************************************************
//								INCLUDES
//******************************************************************************
#include "../../core/include/serial_interface.h"
#include <errno.h>
#include <windows.h>

//******************************************************************************
//								VARIABLES
//******************************************************************************


//******************************************************************************
//								TYPES
//******************************************************************************
class WindowsSerial : SerialInterface
{
private:
	std::string name;
	SerialOptions opts;
	HANDLE hSerial;
	std::string get_valid_serial_name(const std::string& comname);
public:
	WindowsSerial() : name(""), opts(), hSerial(0)
	{}
	~WindowsSerial()
	{
		if (hSerial) 
		{
			close();
		}
	}
	
	std::string get_name();
	
	void open(const SerialOptions& opts);
	void close();
	void write(const uint8_t* const src, size_t size);
	void read(uint8_t* const dst, size_t size);	
};

#endif /* _WIN_SERIAL_H_ */