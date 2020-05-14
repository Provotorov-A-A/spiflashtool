#ifndef _WIN_SERIAL_H_
#define _WIN_SERIAL_H_
//******************************************************************************
//								INCLUDES
//******************************************************************************
#include "../../core/include/interfaces/serial_interface.h"
#include <errno.h>
#include <windows.h>

//******************************************************************************
//								VARIABLES
//******************************************************************************


//******************************************************************************
//								TYPES
//******************************************************************************
class WindowsSerial : public SerialInterface
{
private:
	std::string name;
	SerialOptions opts;
	HANDLE hSerial;
	
	std::string get_valid_serial_name(const std::string& comname);
	void open();
	void close();
	
public:
	WindowsSerial(const SerialOptions& opts) : name(""), hSerial(0)
	{
		this->opts = opts;
		open();
	}
	
	~WindowsSerial()
	{
		if (hSerial) 
		{
			close();
		}
	}
	
	std::string get_name();
	
	void write(const uint8_t* const src, const size_t size);
	void read(uint8_t* const dst, const size_t size);	
};

#endif /* _WIN_SERIAL_H_ */