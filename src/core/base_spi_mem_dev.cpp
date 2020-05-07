//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <cstdint>
#include <cstring>
#include <string>

#include "include/base_spi_mem_dev.h"

//******************************************************************************
//								PROCEDURES
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDevice::erase_sector(const uint32_t dst_address) const
{
	uint8_t out_data_header[4];
	uint8_t in_data[] = {0x00, 0x00, 0x00, 0x00};
	
	out_data_header[0] = SpiFlashProtoCommands::SPIFLASH_COMMAND_SECTOR_ERASE;
	out_data_header[1] = dst_address >> 16;
	out_data_header[2] = dst_address >> 8;
	out_data_header[3] = dst_address;
	
	spi_if->set_CS(CS_STATE_ACTIVE);
	spi_if->data_xfer(out_data_header, in_data, sizeof(out_data_header));
	spi_if->set_CS(CS_STATE_INACTIVE);
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDevice::program_page(const uint32_t dst_address, const uint8_t* const src, const size_t size) const
{
	uint8_t out_data_header[4];
	
	if ( (0 == size) || (0 == src) )
	{
		return;
	}
	
	out_data_header[0] = SpiFlashProtoCommands::SPIFLASH_COMMAND_PAGE_PROGRAM;
	out_data_header[1] = dst_address >> 16;
	out_data_header[2] = dst_address >> 8;
	out_data_header[3] = dst_address;
	
	uint8_t* in_data = new uint8_t[size + sizeof(out_data_header)];
	if (!in_data) 
	{
		throw SpiMemoryError("memory allocation error");
		return;
	}
	
	spi_if->set_CS(CS_STATE_ACTIVE);
	spi_if->data_xfer(out_data_header, 	in_data, 							sizeof(out_data_header));
	spi_if->data_xfer(src,		 		in_data+sizeof(out_data_header), 	size);
	spi_if->set_CS(CS_STATE_INACTIVE);		
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDevice::read_data(const uint32_t src_address, uint8_t* const dst, const size_t size) const
{
	uint8_t out_data_header[4];
	uint8_t dummy_in_data[4];
	
	if ( (0 == size) || (0 == dst) )
	{
		return;
	}
	
	out_data_header[0] = SpiFlashProtoCommands::SPIFLASH_COMMAND_READ_DATA;
	out_data_header[1] = src_address >> 16;
	out_data_header[2] = src_address >> 8;
	out_data_header[3] = src_address;
	
	uint8_t* out_data = new uint8_t[size];
	if (!out_data) 
	{
		throw SpiMemoryError("memory allocation error");
		return;
	}
	memset(out_data, 0xFF, size);
	
	spi_if->set_CS(CS_STATE_ACTIVE);
	spi_if->data_xfer(out_data_header, 	dummy_in_data, 	sizeof(out_data_header));
	spi_if->data_xfer(out_data, 		dst, 			size);
	spi_if->set_CS(CS_STATE_INACTIVE);	
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t BaseSpiMemoryDevice::read_id() const
{
	uint8_t out_data[] = {SpiFlashProtoCommands::SPIFLASH_COMMAND_READ_ID, 0xFF, 0xFF, 0xFF};
	uint8_t in_data[] = {0x00, 0x00, 0x00, 0x00};
	
	spi_if->set_CS(CS_STATE_ACTIVE);
	spi_if->data_xfer(out_data, in_data, sizeof(out_data));
	spi_if->set_CS(CS_STATE_INACTIVE);
	
	return *((uint32_t*)(in_data+1));
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDevice::write_enable() const
{
	uint8_t out_data_header[1] = {SpiFlashProtoCommands::SPIFLASH_COMMAND_WRITE_ENABLE};
	uint8_t dummy_in_data[1];

	spi_if->set_CS(CS_STATE_ACTIVE);
	spi_if->data_xfer(out_data_header, dummy_in_data, sizeof(out_data_header));
	spi_if->set_CS(CS_STATE_INACTIVE);
	
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDevice::write_disable() const
{
	uint8_t out_data_header[1] = {SpiFlashProtoCommands::SPIFLASH_COMMAND_WRITE_DISABLE};
	uint8_t dummy_in_data[1];

	spi_if->set_CS(CS_STATE_ACTIVE);
	spi_if->data_xfer(out_data_header, dummy_in_data, sizeof(out_data_header));
	spi_if->set_CS(CS_STATE_INACTIVE);	
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint8_t BaseSpiMemoryDevice::read_status_reg() const
{
	uint8_t out_data[] = {SpiFlashProtoCommands::SPIFLASH_COMMAND_READ_STATUS_REG, 0xFF};
	uint8_t in_data[] = {0x00, 0x00};
	
	spi_if->set_CS(CS_STATE_ACTIVE);
	spi_if->data_xfer(out_data, in_data, sizeof(out_data));
	spi_if->set_CS(CS_STATE_INACTIVE);
	
	return in_data[1];
}

//==============================================================================
