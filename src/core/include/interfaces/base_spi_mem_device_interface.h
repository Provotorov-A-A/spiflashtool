#ifndef  _BASE_SPI_MEM_DEVICE_INTERFACE_H_
#define _BASE_SPI_MEM_DEVICE_INTERFACE_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <cstdint>
#include <string>

#include "common_exception.h"

//******************************************************************************
//								TYPES
//******************************************************************************
//==============================================================================
class SpiMemoryError : CommonException
{
public:
	SpiMemoryError () : CommonException("spi memory interface","") 
	{};
	SpiMemoryError (const std::string& s) : CommonException("spi memory interface", s) 
	{};
};

//==============================================================================
class BaseSpiMemoryDeviceInterface
{
public:
	virtual size_t get_total_size() const = 0;
	virtual size_t get_wr_page_size() const = 0;
	virtual size_t get_er_sector_size() const = 0;
	
	virtual void erase_sector(const uint32_t dst_address_inside_sector) = 0;
	virtual void program_page(const uint32_t dst_address, const uint8_t* const src, const size_t size) = 0;
	virtual void read_data(const uint32_t src_address, uint8_t* const dst, const size_t size) = 0;
	virtual uint32_t read_id() = 0;
	virtual void write_enable() = 0;
	virtual void write_disable() = 0;
	virtual uint8_t read_status_reg() = 0;
	
	virtual ~BaseSpiMemoryDeviceInterface() {};
};

//==============================================================================

#endif /* _BASE_SPI_MEM_DEVICE_INTERFACE_H_ */