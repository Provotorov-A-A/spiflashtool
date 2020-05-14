#ifndef  _SPI_DEVICE_IF_H_
#define _SPI_DEVICE_IF_H_

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
enum CS_State
{
	CS_STATE_ACTIVE,
	CS_STATE_INACTIVE = !CS_STATE_ACTIVE
};

//==============================================================================
class SpiDeviceInterface
{
public:
	virtual void set_CS(const CS_State state) const = 0;
	virtual void data_xfer(const uint8_t* const src, uint8_t* const dst, const size_t size) = 0;
};
//==============================================================================


#endif /* _SPI_DEVICE_IF_H_ */