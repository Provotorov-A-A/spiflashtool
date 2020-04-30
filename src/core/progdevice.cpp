
//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <string>
#include "include/common_exception.h"
#include "include/progdevice.h"
#include <cstring>

//******************************************************************************
//								TYPES
//******************************************************************************

//******************************************************************************
//								PROCEDURES
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ProgDevice::util_link() const
{
	uint8_t data[1] = {PROGDEV_PROTO_HEADER_CMD_LINK};
	uint8_t resp[1] = {0};
	
	data[0] = PROGDEV_PROTO_HEADER_CMD_LINK;
	
	serial->write(data, sizeof(data));
	serial->read(resp, 1);
	
	if (PROGDEV_PROTO_HEADER_RESP_LINK != resp[0])
	{
		throw ProgDevice_Error("link command failed");
	}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ProgDevice::util_exit_to(const uint32_t address) const
{
	uint8_t data[1] = {PROGDEV_PROTO_HEADER_CMD_EXIT};
	uint8_t resp[2] = {0x00, 0x00};

	serial->write(data, sizeof(data));
	serial->read(resp, 2);
	
	if ( !( (PROGDEV_PROTO_STATUS_RESP_OK == resp[1]) && (PROGDEV_PROTO_HEADER_RESP_EXIT == resp[0]) ) )
	{
		throw ProgDevice_Error("exit to address command failed");
	}
};

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ProgDevice::util_set_baudrate(const uint32_t b) const
{
	// FIXME: implement util_set_baudrate
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ProgDevice::set_CS(const CS_State state)  const
{
	uint8_t data[1];
	uint8_t resp[1] = {0};
	
	if (CS_STATE_ACTIVE == state)
	{
		data[0] = PROGDEV_PROTO_HEADER_CMD_CS_SET;
	}
	else
	{
		data[0] = PROGDEV_PROTO_HEADER_CMD_CS_CLR;
	}

	serial->write(data, sizeof(data));
	serial->read(resp, 1);

	if 	( ( (CS_STATE_ACTIVE == state) && (PROGDEV_PROTO_HEADER_RESP_CS_SET != resp[0]) ) ||
		( (CS_STATE_INACTIVE == state) && (PROGDEV_PROTO_HEADER_RESP_CS_CLR != resp[0]) ) )
	{
		throw ProgDevice_Error("set CS command failed");
	}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ProgDevice::data_xfer(const uint8_t* const src, uint8_t* const dst, const size_t data_size)
{
	uint8_t out_header[3];
	uint8_t* inbuf;
	size_t 	no_crc_in_size;

	// Allocate receive buffer (response header + status + data + checksum).
	no_crc_in_size = 1 + 1 + data_size;
	
	try
	{
		inbuf = new uint8_t[no_crc_in_size];
	}
	catch (std::bad_array_new_length& e)
	{
		throw ProgDevice_Error("memory allocation failed");
	}

	try
	{
		memset(inbuf, 0xEE, no_crc_in_size);
		// Send start transaction command and size first
		out_header[0] = PROGDEV_PROTO_HEADER_CMD_START;
		out_header[1] = data_size & 0xFF;
		out_header[2] = (data_size>>8) & 0xFF;
		
		serial->write(out_header, sizeof(out_header));
		
		// Send data
		serial->write(src, data_size);
		
		// Send checksum
		if (opts.check_crc)
		{
			uint16_t checksum = crc16_calc(src, data_size);
			serial->write((uint8_t*)&checksum, sizeof(checksum));			// FIXME: Check Endiannes!
		}
		
	//--------------------------------------------------------------------------
	// Read
		// Read all of expected data
		serial->read(inbuf, no_crc_in_size);

		// Check header and status
		if ( (inbuf[0] != PROGDEV_PROTO_HEADER_RESP_START) || (inbuf[1] != PROGDEV_PROTO_STATUS_RESP_OK) )
		{
			throw ProgDevice_Error("invalid response status or header");
		}

		if (opts.check_crc)
		{
			uint16_t rec_crc16 = 0x0000;
			serial->read((uint8_t*)&rec_crc16, sizeof(rec_crc16));		// FIXME: Check Endiannes!
			// Checksum test
			if (crc16_calc(inbuf+2, data_size) != rec_crc16)
			{
				throw ProgDevice_Error("invalid response crc");
			}
		}
		// Copy received data to destination if all is OK
		memcpy(dst, inbuf+2, data_size);
	}
	catch (HW_IO_Error& e)
	{
		delete [] inbuf;
		throw ProgDevice_Error("data transfer command failed because of hardware interface error");
	}
	catch (ProgDevice_Error& e)
	{
		throw ProgDevice_Error(e.what());
	}
	catch (...)
	{
		delete [] inbuf;
		throw ProgDevice_Error("");
	}
	
	delete [] inbuf;
}













