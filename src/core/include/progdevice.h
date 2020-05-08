#ifndef  _PROGDEVICE_H_
#define _PROGDEVICE_H_

/*
	Description:
	
	CMD FORMAT:
	| CMD_LINK				(1)		|				|							==>		| RESPONSE_LINK	(1)						|
	| CMD_TRANSACTION_START	(1)		|	SIZE	(2)	|	DATA_OUT(1..512)	|	==>		| RESPONSE_TRANSACTION_START	(1)		|	STATUS	(1)	|	DATA_IN (1..256) (IF STATUS OK)	|
	| CMD_EXIT				(1)		|	ADDRESS (8)	|							==>		| RESPONSE_EXIT 				(1)		|	STATUS	(1)	|
	| CMD_BAUDRATE			(1)		|	BAUDRATE(4)	|							==>		| RESPONSE_BAUDRATE				(1)		|	STATUS	(1)	|
	| CMD_CS_SET			(1)		|				|							==>		| RESPONSE_CS_SET				(1)		|	
	| CMD_CS_CLEAR			(1)		|				|							==>		| RESPONSE_CS_CLEAR				(1)		|	
	
	* NOTE: RESPONSE_BAUDRATE response and STATUS for that command transmitted on an old baudrate. Bootloader checks if it can set this speed and return STATUS_OK if it can.
*/

//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <cstdint>
#include <string>

#include "common_exception.h"
#include "spi_dev_if.h"
#include "serial_interface.h"

//******************************************************************************
//								TYPES
//******************************************************************************

static const std::string operation_enum_to_string[] = {"NOT_SPECIFIED", "READ_MEM", "WRITE_MEM", "READ_JEDEC_ID", "ERASE_MEM"};

//==============================================================================
struct ProgDeviceOptions
{
	bool 					check_crc;
	bool 					verbose;		// FIXME: use verbose option in operations
	
	ProgDeviceOptions() : 	check_crc(false), verbose(false)
	{ };
							
	ProgDeviceOptions(const ProgDeviceOptions& opts) : 	check_crc(opts.check_crc), verbose(opts.verbose)
	{};

	void print() const
	{
		std::cout << "	check_crc 	: " << check_crc << std::endl;
		std::cout << "	verbose 	: " << verbose << std::endl;
	};
};

//==============================================================================
class ProgDevice_Error : public CommonException
{
public:
	ProgDevice_Error () : CommonException("programming device protocol", "")
	{};
	ProgDevice_Error (const std::string& s) : CommonException("programming device protocol",s)
	{};
};

//==============================================================================
class ProgDevice : public SpiDeviceInterface
{
private:
	enum ProgDeviceProtocolEnum : uint8_t
	{
		PROGDEV_PROTO_HEADER_CMD_LINK		= 'L',
		PROGDEV_PROTO_HEADER_CMD_START 		= 'S',
		PROGDEV_PROTO_HEADER_CMD_EXIT		= 'E',
		PROGDEV_PROTO_HEADER_CMD_BAUDRATE	= 'B',
		PROGDEV_PROTO_HEADER_CMD_CS_SET		= 'O',
		PROGDEV_PROTO_HEADER_CMD_CS_CLR		= 'Z',

		PROGDEV_PROTO_HEADER_RESP_LINK		= PROGDEV_PROTO_HEADER_CMD_LINK,
		PROGDEV_PROTO_HEADER_RESP_START 	= PROGDEV_PROTO_HEADER_CMD_START,
		PROGDEV_PROTO_HEADER_RESP_EXIT		= PROGDEV_PROTO_HEADER_CMD_EXIT,
		PROGDEV_PROTO_HEADER_RESP_BAUDRATE	= PROGDEV_PROTO_HEADER_CMD_BAUDRATE,
		PROGDEV_PROTO_HEADER_RESP_CS_SET	= PROGDEV_PROTO_HEADER_CMD_CS_SET,
		PROGDEV_PROTO_HEADER_RESP_CS_CLR	= PROGDEV_PROTO_HEADER_CMD_CS_CLR,
		
		PROGDEV_PROTO_STATUS_RESP_OK		= 'K',
		PROGDEV_PROTO_STATUS_RESP_ERROR		= 'R'
	} ;

	ProgDeviceOptions 		opts;
	SerialInterface*		serial;
	
	uint16_t crc16_calc(const uint8_t* const data, const size_t size) const {return 0x0000;};		// FIXME: implement crc16 function
	
public:
	ProgDevice( SerialInterface* const pSerial);
	
	// spi device interface methods
	void set_CS(const CS_State state)  const;
	void data_xfer(const uint8_t* const src, uint8_t* const dst, const size_t size);
	
	// programmer specific aux methods
	void util_link() const;
	void util_exit_to(const uint32_t address) const;
	void util_set_baudrate(const uint32_t b) const;		
};

#endif /* _PROGDEVICE_H_ */