
//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <fstream>
#include <string>
#include <iomanip>
#include "include/base_spi_mem_driver_impl.h"

//******************************************************************************
//								TYPES
//******************************************************************************

//******************************************************************************
//								PROCEDURES
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDriver::read_mem_to_file(const std::string& filename, const uint32_t from_address, const size_t size)
{
	if (0 == size)
	{
		return;
	}

	if ( (from_address >= spiflash_dev->get_total_size()) || (from_address + size > spiflash_dev->get_total_size()) )
	{
		throw SpiMemoryDriverError("bad address or size for reading");
		return;
	}
	
	uint32_t mem_address_start = from_address;
	size_t total_size = size;
	uint32_t mem_address_end = mem_address_start + total_size - 1;					// inclusive
	
	//--------------------------------------------------------------------------
	// Open file to write received data
	std::ofstream outfile;
	try
	{
		outfile.open(filename, std::ios::out | std::ios::binary);
	}
	catch (...)
	{
		throw new SpiMemoryDriverError("open file to write error");
		return;
	}

	//--------------------------------------------------------------------------
	// Read data from Flash device and write it to file
	unsigned int progress = 0;
	std::cout << "Read memory:" << std::endl;
	std::cout << std::setbase(10) << total_size << " byte(s) from " << std::setbase(16) << std::showbase << mem_address_start << " to " << mem_address_end << std::endl;
	std::cout << "Progress: ";
	std::cout << std::setw(3) << std::right << std::setbase(10) << progress << "%";
	std::cout.flush();
	
	uint32_t cur_address = mem_address_start;
	size_t received = 0;
	size_t rest = size;
	// Choose size for received data buffering before writing it to file. May be any value > 0.
	size_t data_portion_size_max = spiflash_dev->get_wr_page_size();		
	size_t data_portion_size = 0;
	uint8_t* data = new uint8_t[data_portion_size_max];
	
	if (!data)
	{
		throw SpiMemoryDriverError("memory allocation error");
		return;
	}
	try
	{
		do 
		{
			data_portion_size = (rest > data_portion_size_max)? data_portion_size_max : rest;
			
			// read memory when it will be ready
			if (!wait_ready())
			{
				throw SpiMemoryDriverError("wait memory ready timeout error");		
			}
			spiflash_dev->read_data(cur_address, data, data_portion_size);
			outfile.write((char*)data, data_portion_size);
			if (!outfile.good())
			{
				outfile.close();
				throw SpiMemoryDriverError("file write error");
				return;
			}
			
			received += data_portion_size;
			progress = (received * 100 ) / size;
			std::cout << "\b\b\b\b" << std::setw(3) << std::right << std::setbase(10) << progress << "%";
			std::cout.flush();
			cur_address += data_portion_size;
			rest -= data_portion_size;
		}
		while (rest > 0);
		
		std::cout << std::endl;
	}
	catch (...)
	{
		std::cout << std::endl;
		outfile.flush();
		delete [] data;
		outfile.close();
		throw SpiMemoryDriverError("memory read error occured on " + std::to_string(progress) + "% progress");		
	}

	outfile.flush();	
	delete [] data;
	outfile.close();	
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDriver::write_mem_from_file(const std::string& filename, const uint32_t address, const size_t size)
{
	size_t fsize = 0;
	
	//--------------------------------------------------------------------------
	// Test memory region availability
	if ( (address >= spiflash_dev->get_total_size()) || (address + size > spiflash_dev->get_total_size()) )
	{
		throw SpiMemoryDriverError("bad address or size for programming");
		return;
	}
	
	//--------------------------------------------------------------------------
	// Open file to read data, get file size
	std::ifstream file;
	try
	{
		file.open(filename, std::ios::in | std::ios::binary);
		fsize = filesize(filename.c_str());
		if ( (0 == size) && (0 == fsize) )
		{
			throw SpiMemoryDriverError("");
		}
	}
	catch (...)
	{
		throw SpiMemoryDriverError("error while trying to open file");
		return;
	}
	
	if ((0 != size) && (size > fsize)) 
	{
		throw SpiMemoryDriverError("file size is too small");
		return;
	}

	size_t total_data_size = (size)?  size : fsize;

	size_t FLASH_DEVICE_PROGRAM_PAGE_SIZE = spiflash_dev->get_wr_page_size();
	uint32_t mem_address_start = address;
	uint32_t mem_address_end = mem_address_start + total_data_size - 1;					// inclusive

	//--------------------------------------------------------------------------
	unsigned int progress = 0;
	std::cout << "Program memory:" << std::endl;
	std::cout << std::setbase(10) << total_data_size << " byte(s) from " << std::setbase(16) << std::showbase << mem_address_start << " to " << mem_address_end << std::endl;
	std::cout << "Progress: " ;
	std::cout << std::left << std::setbase(10) << std::setw(3) << progress << "%";
	

	//--------------------------------------------------------------------------
	size_t data_transmitted = 0;
	size_t cur_address = mem_address_start;
	size_t rest = total_data_size;
	size_t data_portion;
	
	uint8_t* data = new uint8_t[FLASH_DEVICE_PROGRAM_PAGE_SIZE];
	if (!data)
	{
		throw SpiMemoryDriverError("memory allocation error");
		return;
	}
	
	try
	{
		
		while(data_transmitted < total_data_size)
		{
			// Calculate data size to be read from file and programed to flash device
			size_t cur_page_start = cur_address - cur_address % FLASH_DEVICE_PROGRAM_PAGE_SIZE;
			size_t next_page_start = cur_page_start + FLASH_DEVICE_PROGRAM_PAGE_SIZE;
			
			data_portion = FLASH_DEVICE_PROGRAM_PAGE_SIZE;	// default
			if (cur_address != cur_page_start)				// if start address is not the start of page
			{
				data_portion = next_page_start - cur_address;
			}
			if ( (cur_address + rest ) <= next_page_start)	// if end address is not the end of page
			{
				data_portion = rest;
			}
			
			file.read((char*)data, data_portion);
			if (!file.good())
			{
				file.close();
				throw SpiMemoryDriverError("file read error");
				return;
			}

			// Send write enable first
			if (!wait_ready())
			{
				throw SpiMemoryDriverError("wait memory ready timeout error");		
			}
			spiflash_dev->write_enable();
	
			if (!wait_ready())
			{
				throw SpiMemoryDriverError("wait memory ready timeout error");		
			}
			spiflash_dev->program_page(cur_address, data, data_portion);
			
			data_transmitted += data_portion;
			progress = (data_transmitted * 100 ) / total_data_size;
			std::cout << "\b\b\b\b" << std::setw(3) << std::right << std::setbase(10) << progress << "%";
			
			cur_address += data_portion;
			rest -= data_portion;
		}
	}
	catch(...)
	{
		std::cout << std::endl;
		delete [] data;
		file.close();
		throw SpiMemoryDriverError("memory write error occured on " + std::to_string(progress) + "% progress");		
	}
	
	std::cout << std::endl;
	delete [] data;
	file.close();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiMemoryDriver::erase(const uint32_t address, const size_t size)
{
	if (0 == size)
	{
		return;
	}
	
	if ( (address >= spiflash_dev->get_total_size()) || (address + size > spiflash_dev->get_total_size()) )
	{
		throw SpiMemoryDriverError("bad address or size for erasing");
		return;
	}
	
	size_t MEMORY_SECTOR_SIZE = spiflash_dev->get_er_sector_size();
	
	size_t sector_base_address_start 	= address;
	size_t sector_base_address_end 		= address + size;
	
	size_t flash_address_start = sector_base_address_start;
	size_t flash_address_end = sector_base_address_end + MEMORY_SECTOR_SIZE - 1;
	
	size_t sector_num_start = sector_base_address_start / MEMORY_SECTOR_SIZE;
	size_t sector_num_end 	= sector_base_address_end / MEMORY_SECTOR_SIZE;
	size_t sectors_to_erase = (sector_num_end - sector_num_start + 1);

	size_t bytes_to_erase = flash_address_end - flash_address_start + 1;	

	//--------------------------------------------------------------------------
	unsigned int progress = 0;
	std::cout << "Erase memory:" << std::endl;
	std::cout << std::setbase(10) << sectors_to_erase << " sector(s) from #" << sector_num_start << " to #" << sector_num_end << std::endl;
	std::cout << bytes_to_erase << " byte(s) from " << std::setbase(16) << std::showbase << flash_address_start << " to " << flash_address_end << std::endl;;
	std::cout << "Progress: " ;
	std::cout << std::right << std::setbase(10) << std::setw(3) << progress << "%";
	std::cout.flush();
	
	//--------------------------------------------------------------------------
	size_t cur_address = sector_base_address_start;
	try
	{	
		for (size_t sector_num = sector_num_start; sector_num <= sector_num_end; ++sector_num)
		{
			// Send write enable first
			if (!wait_ready())
			{
				throw SpiMemoryDriverError("wait memory ready timeout error");		
			}
			spiflash_dev->write_enable();

			// erase memory when it will be ready
			if (!wait_ready())
			{
				throw SpiMemoryDriverError("wait memory ready timeout error");		
			}
			spiflash_dev->erase_sector(cur_address);
			
			cur_address += MEMORY_SECTOR_SIZE;
			progress = ((sector_num_start + sector_num + 1) * 100) / (sector_num_end - sector_num_start + 1);
			std::cout << "\b\b\b\b" << std::setw(3) << std::right << std::setbase(10) << progress << "%";
			std::cout.flush();
		}
	}	
	catch (...)
	{
		throw SpiMemoryDriverError("memory erase error occured on " + std::to_string(progress) + "% progress");		
		return;
	}	
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t BaseSpiMemoryDriver::read_id()
{
	return spiflash_dev->read_id();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool BaseSpiMemoryDriver::wait_ready()
{
	unsigned int tries = 0;
	while(true)
	{
		uint8_t status = spiflash_dev->read_status_reg();
		if (0 == (status & STATUS_REGISTER_BITS_BUSY))
		{
			return true;
		}
		else
		{
			if (WAIT_READY_TRIES_MAX == tries++)
			{
				return false;
			};
		}
	};	
}