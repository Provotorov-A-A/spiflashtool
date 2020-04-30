#ifndef  _PROGDEVICE_H_
#define _PROGDEVICE_H_

/*
	Description:
	
	CMD FORMAT:
	| CMD_LINK				(1)		|				|							==>		| RESPONSE_LINK	(1)						|
	| CMD_TRANSACTION_START	(1)		|	SIZE	(2)	|	DATA_OUT(1..256)	|	==>		| RESPONSE_TRANSACTION_START	(1)		|	STATUS	(1)	|	DATA_IN (1..256) (IF STATUS OK)	|
	| CMD_EXIT				(1)		|	ADDRESS (8)	|							==>		| RESPONSE_EXIT 				(1)		|	STATUS	(1)	|
	| CMD_BAUDRATE			(1)		|	BAUDRATE(4)	|							==>		| RESPONSE_BAUDRATE				(1)		|	STATUS	(1)	|
	
	* NOTE: RESPONSE_BAUDRATE response and STATUS for that command transmitted on an old baudrate. Bootloader checks if it can set this speed and return STATUS_OK if it can.
*/

//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <cstdint>
#include <string>

#include "common_exception.h"

//******************************************************************************
//								TYPES
//******************************************************************************
typedef enum
{
	USA_TRANSACTION_STATUS_OK 				= 0x01,
	USA_TRANSACTION_STATUS_INVALID 			= 0x02,
	USA_TRANSACTION_STATUS_HW_ERROR			= 0x04,
} uart_spi_adapter_transaction_status;


enum CS_State
{
	CS_STATE_ACTIVE,
	CS_STATE_INACTIVE = !CS_STATE_ACTIVE
};

static const std::string operation_enum_to_string[] = {"NOT_SPECIFIED", "READ_MEM", "WRITE_MEM", "READ_JEDEC_ID", "ERASE_MEM"};

//==============================================================================
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
							
	ProgDeviceOptions(const ProgDeviceOptions& opts) : 	address(opts.address), size(opts.size), filename(opts.filename), 
														check_crc(opts.check_crc), verbose(opts.verbose)
	{};
	
	ProgDeviceOptions& operator= (const ProgDeviceOptions& opts)
	{
		this->address = opts.address;
		this->size = opts.size;
		this->filename = opts.filename;
		this->check_crc = opts.check_crc;
		this->verbose = opts.verbose;
		
		return *this;
	};
	
	void print() const
	{
		std::cout << "	address 	: " << address << std::endl;
		std::cout << "	size 		: " << size << std::endl;
		std::cout << "	filename 	: " << filename << std::endl;
		std::cout << "	check_crc 	: " << check_crc << std::endl;
		std::cout << "	verbose 	: " << verbose << std::endl;
	};
};

//==============================================================================
class ProgDevice
{
private:
	enum
	{
		USA_PROTO_HEADER_LINK		= 'L',
		USA_PROTO_HEADER_CMD_START 	= 'S',
		USA_PROTO_HEADER_EXIT		= 'E',
		USA_PROTO_HEADER_BAUDRATE	= 'B'
	} uart_spi_adapter_proto_header;

	typedef enum
	{
		USA_PROTO_STATUS_OK 		= 0x04,
		USA_PROTO_STATUS_FAIL		= 0xEE
	} uart_spi_adapter_proto_status;

	ProgDeviceOptions opts;
	
public:
	void exit_to(const uint32_t address) const;
	void set_CS(const CS_State state)  const;
	void write_from_file(const std::string& f, const uint32_t address, const size_t size)  const;
	void read_to_file(const std::string& f, const uint32_t address, const size_t size)  const;
	void erase(const uint32_t address, const size_t size)  const;
	std::string read_jedec_id()  const;
	ProgDeviceOptions& get_options()  const;
	void set_options(const ProgDeviceOptions& opts);
};

//==============================================================================
// void ProgDevice::exit_to(const uint32_t address) const
// {
	
// };





#endif /* _PROGDEVICE_H_ */