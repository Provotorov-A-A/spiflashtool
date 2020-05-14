#ifndef  _BASE_SPI_MEM_DEVICE_IMPL_H_
#define _BASE_SPI_MEM_DEVICE_IMPL_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <cstdint>
#include <string>

#include "common_exception.h"
#include "interfaces/spi_device_interface.h"
#include "interfaces/base_spi_mem_device_interface.h"

//******************************************************************************
//								TYPES
//******************************************************************************
#define SPI_FLASH_MAX_CAPACITY		0x1000000

enum SpiFlashProtoCommands : uint8_t
{
	SPIFLASH_COMMAND_WRITE_ENABLE = 0x06,
	SPIFLASH_COMMAND_WRITE_DISABLE = 0x04,
	SPIFLASH_COMMAND_READ_ID = 0x9F,
	SPIFLASH_COMMAND_READ_STATUS_REG = 0x05,
	SPIFLASH_COMMAND_WRITE_STATUS_REG = 0x01,
	SPIFLASH_COMMAND_READ_DATA = 0x03,
	SPIFLASH_COMMAND_PAGE_PROGRAM = 0x02,
	SPIFLASH_COMMAND_SECTOR_ERASE = 0x20
} ;

//==============================================================================
class BaseSpiMemoryDevice : public BaseSpiMemoryDeviceInterface
{
private:
	SpiDeviceInterface* spi_if;
	size_t write_page_size;
	size_t erase_sector_size;
	
public:

	BaseSpiMemoryDevice(SpiDeviceInterface* spi_if, 
						size_t page_size = 512, 
						size_t sector_size = 4096) : 	spi_if(spi_if),
														write_page_size(page_size), 
														erase_sector_size(sector_size) 
	{ if (0 == spi_if) throw new SpiMemoryError("illegal spi_device_interface instance");};
	
	size_t get_total_size() const {return SPI_FLASH_MAX_CAPACITY;} ;
	size_t get_wr_page_size() const {return write_page_size;};
	size_t get_er_sector_size() const {return erase_sector_size;};
	
	void erase_sector(const uint32_t dst_address_inside_sector);
	void program_page(const uint32_t dst_address, const uint8_t* const src, const size_t size);
	void read_data(const uint32_t src_address, uint8_t* const dst, const size_t size);
	uint32_t read_id();
	void write_enable();
	void write_disable();
	uint8_t read_status_reg();
};

//==============================================================================

#endif /* _BASE_SPI_MEM_DEVICE_IMPL_H_ */