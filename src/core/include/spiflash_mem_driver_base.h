#ifndef  _SPIFLASH_MEM_DRIVER_BASE_H_
#define _SPIFLASH_MEM_DRIVER_BASE_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <cstdint>
#include <string>

#include "common_exception.h"
#include "base_spi_mem_dev.h"

//******************************************************************************
//								TYPES
//******************************************************************************
//==============================================================================
class SpiMemoryDriverError : public CommonException
{
public:
	SpiMemoryDriverError () : CommonException("spi flash memory driver","") 
	{};
	SpiMemoryDriverError (const std::string& s) : CommonException("spi memory driver", s) 
	{};
};

//==============================================================================
class BaseSpiFlashMemoryDriver
{
private:
	bool verbose;
	BaseSpiMemoryDevice* spiflash_dev;
	
public:
	BaseSpiFlashMemoryDriver(BaseSpiMemoryDevice* const mem_dev, const bool verbose = true) : 	verbose(verbose), 
																								spiflash_dev(mem_dev)
	{
		if (0 == mem_dev) throw new SpiMemoryDriverError("illegal spiflash device instance");
	};

	void read_mem_to_file(const std::string& filename, const uint32_t from_address, const size_t size);
	void write_mem_from_file(const std::string& filename, const uint32_t to_address, const size_t size = 0);
	void erase(const uint32_t from_address, const size_t size);
	uint32_t read_id();
};

//==============================================================================

#endif /* _SPIFLASH_MEM_DRIVER_BASE_H_ */