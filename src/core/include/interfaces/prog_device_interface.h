#ifndef  _PROG_DEVICE_INTERFACE_H_
#define _PROG_DEVICE_INTERFACE_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <cstdint>
#include <string>

#include "common_exception.h"
#include "spi_device_interface.h"

//******************************************************************************
//								TYPES
//******************************************************************************
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
class ProgDeviceInterface : public SpiDeviceInterface
{
public:
	// spi device interface methods
	virtual void set_CS(const CS_State state)  const = 0;
	virtual void data_xfer(const uint8_t* const src, uint8_t* const dst, const size_t size) = 0;
	
	// programmer specific aux methods
	virtual void util_link() const = 0;
	virtual void util_exit_to(const uint32_t address) const = 0;
	virtual void util_set_baudrate(const uint32_t b) const = 0;		
	virtual ~ProgDeviceInterface() {};
};

#endif /* _PROG_DEVICE_INTERFACE_H_ */