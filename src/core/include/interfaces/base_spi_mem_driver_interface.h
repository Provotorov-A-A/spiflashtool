#ifndef  _BASE_SPI_MEM_DRIVER_INTERFACE_H_
#define _BASE_SPI_MEM_DRIVER_INTERFACE_H_

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
class SpiMemoryDriverError : public CommonException
{
public:
	SpiMemoryDriverError () : CommonException("spi flash memory driver","") 
	{};
	SpiMemoryDriverError (const std::string& s) : CommonException("spi flash memory driver", s) 
	{};
};

//==============================================================================
class BaseSpiMemoryDriverInterface
{
public:
	virtual void read_mem_to_file(const std::string& filename, const uint32_t from_address, const size_t size) = 0;
	virtual void write_mem_from_file(const std::string& filename, const uint32_t to_address, const size_t size = 0) = 0;
	virtual void erase(const uint32_t from_address, const size_t size) = 0;
	virtual uint32_t read_id() = 0;
};

//==============================================================================

#endif /* _BASE_SPI_MEM_DRIVER_INTERFACE_H_ */