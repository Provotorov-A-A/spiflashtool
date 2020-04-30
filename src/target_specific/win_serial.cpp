//******************************************************************************
//								INCLUDES
//******************************************************************************
#include "../core/include/serial_interface.h"
#include "include/win_serial.h"

//******************************************************************************
//								TYPES
//******************************************************************************

//******************************************************************************
//								VARIABLES
//******************************************************************************


//******************************************************************************
//								PROCEDURES
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string WindowsSerial::get_name()
{
	return WindowsSerial::name;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WindowsSerial::open()
{
    COMMTIMEOUTS timeouts;
 	DCB dcbSerialParams;
 	
	std::string name = get_valid_serial_name(opts.name);
	
    hSerial = CreateFile( 	name.data(), 
							GENERIC_READ | GENERIC_WRITE, 
							0, 							// must be opened with exclusive-access
							NULL, 						// default security attributes
							OPEN_EXISTING, 				//  must use OPEN_EXISTING
							0, 							//  not overlapped I/O
							NULL						//  hTemplate must be NULL for comm devices
						);
 
    if ( hSerial == INVALID_HANDLE_VALUE ) 
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
			throw HW_IO_Error("open error");
        }
        throw HW_IO_Error("");;
    }
    
    if (!GetCommState(hSerial, &dcbSerialParams)) 
    {
    	throw HW_IO_Error("read configure error");
    }
 
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
 
    dcbSerialParams.BaudRate = opts.baudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = opts.stopbits;
    dcbSerialParams.Parity = opts.parity;
 
    dcbSerialParams.fBinary = TRUE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
    dcbSerialParams.fOutxCtsFlow = FALSE;
    dcbSerialParams.fOutxDsrFlow = FALSE;
    dcbSerialParams.fDsrSensitivity= FALSE;
    dcbSerialParams.fAbortOnError = TRUE;
 
    if (!SetCommState(hSerial, &dcbSerialParams)) 
    {
    	CloseHandle(hSerial);
    	throw HW_IO_Error("write configure error");;
    }
 
	if (!GetCommTimeouts(hSerial, &timeouts))
	{
		CloseHandle(hSerial);
		throw HW_IO_Error("read configure error");
	}
    
    timeouts.ReadIntervalTimeout = 100;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier= 10;
 
    if(!SetCommTimeouts(hSerial, &timeouts)) 
    {
    	CloseHandle(hSerial);
        throw HW_IO_Error("write configure error");;
    }
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WindowsSerial::close()
{
	CloseHandle(hSerial);
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WindowsSerial::read(uint8_t* const pdata, const size_t size)
{
	DWORD dwBytesRead = 0;
	if ( !ReadFile(hSerial, pdata, size, &dwBytesRead, NULL))
	{
		throw HW_IO_Error("read error");;
	}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WindowsSerial::write(const uint8_t* const pdata, const size_t size)
{
	DWORD dwBytesWritten = 0;
 
    if (!WriteFile(hSerial, pdata, size, &dwBytesWritten, NULL)) 
    { 
		throw HW_IO_Error("read error");;
    }	
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::string WindowsSerial::get_valid_serial_name(const std::string& comname)
{
	const std::string windows_serial_prefix = "\\\\.\\";
	if ( 0 == comname.substr(0, windows_serial_prefix.size()).compare(windows_serial_prefix) )
	{
		return comname;
	}
	else
	{
		return windows_serial_prefix + comname;
	}
}
