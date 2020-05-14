#ifndef  _SPIFLASH_MEM_DRIVER_BASE_IMPL_H_
#define _SPIFLASH_MEM_DRIVER_BASE_IMPL_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <cstdint>
#include <string>

#include "common_exception.h"
#include "interfaces/base_spi_mem_device_interface.h"
#include "interfaces/base_spi_mem_driver_interface.h"

//******************************************************************************
//								TYPES
//******************************************************************************
#define STATUS_REGISTER_BITS_BUSY					(1 << 0)

//==============================================================================
class BaseSpiMemoryDriver : public BaseSpiMemoryDriverInterface
{
private:
	static const unsigned int WAIT_READY_TRIES_MAX = 100;
	bool verbose;
	BaseSpiMemoryDeviceInterface* spiflash_dev;
	bool wait_ready();
public:
	BaseSpiMemoryDriver(BaseSpiMemoryDeviceInterface* const mem_dev, const bool verbose = true) : 	verbose(verbose), 
																							spiflash_dev(mem_dev)
	{
		if (0 == mem_dev) throw SpiMemoryDriverError("illegal spiflash device instance");
	};
	~BaseSpiMemoryDriver() {};

	void read_mem_to_file(const std::string& filename, const uint32_t from_address, const size_t size);
	void write_mem_from_file(const std::string& filename, const uint32_t to_address, const size_t size = 0);
	void erase(const uint32_t from_address, const size_t size);
	uint32_t read_id();
};

//==============================================================================

#endif /* _SPIFLASH_MEM_DRIVER_BASE_IMPL_H_ */