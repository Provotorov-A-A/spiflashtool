
//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <fstream>
#include <string>
#include <iomanip>
#include "include/spiflash_mem_driver_base.h"

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
void BaseSpiFlashMemoryDriver::read_mem_to_file(const std::string& filename, const uint32_t from_address, const size_t size)
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
	std::cout << "Read memory:" << std::endl;
	std::cout << std::setbase(10) << total_size << " byte(s) from " << std::setbase(16) << std::showbase << mem_address_start << " to " << mem_address_end << std::endl;
	std::cout << "Reading: " << std::endl;
	std::cout << std::setw(3) << std::right << std::setbase(10) << 0 << "%";
	
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
			
			spiflash_dev->read_data(cur_address, data, data_portion_size);
			outfile.write((char*)data, data_portion_size);
			if (!outfile.good())
			{
				outfile.close();
				throw SpiMemoryDriverError("file write error");
				return;
			}
			
			received += data_portion_size;
			size_t progress = (received * 100 ) / size;
			std::cout << "\b\b\b" << progress << "%";
			cur_address += data_portion_size;
			rest -= data_portion_size;
		}
		while (rest > 0);
	}
	catch (const std::exception& e)
	{
		std::cout << "error occured!" << std::endl;
		delete [] data;
		outfile.close();
		throw e;
	}

	outfile.flush();	
	delete [] data;
	outfile.close();	
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiFlashMemoryDriver::write_mem_from_file(const std::string& filename, const uint32_t address, const size_t size)
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
	std::cout << "Program memory:" << std::endl;
	std::cout << std::setbase(10) << total_data_size << " byte(s) from " << std::setbase(16) << std::showbase << mem_address_start << " to " << mem_address_end;
	// std::cout << "Progress: " << std::endl;
	// std::cout << std::left << std::setbase(10) << std::setw(3) << 0 << "%";
	
	//--------------------------------------------------------------------------
	// Send write enable first
	spiflash_dev->write_enable();
	
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

		std::cout << "Write " << std::setbase(10) << data_portion << " bytes to " << std::setbase(16) << cur_address << std::endl;
		spiflash_dev->program_page(cur_address, data, data_portion);
		
		data_transmitted += data_portion;
		size_t progress = (data_transmitted * 100 ) / total_data_size;
		//std::cout << "\b\b\b" << progress << "%";
		
		cur_address += data_portion;
		rest -= data_portion;
	}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BaseSpiFlashMemoryDriver::erase(const uint32_t address, const size_t size)
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
	
	size_t sector_base_address_start 	= address 			-	( address 			% MEMORY_SECTOR_SIZE);
	size_t sector_base_address_end 		= (address + size) 	- 	( (address + size) 	% MEMORY_SECTOR_SIZE);
	
	size_t flash_address_start = sector_base_address_start;
	size_t flash_address_end = sector_base_address_end + MEMORY_SECTOR_SIZE - 1;
	
	size_t sector_num_start = sector_base_address_start / MEMORY_SECTOR_SIZE;
	size_t sector_num_end 	= sector_base_address_end / MEMORY_SECTOR_SIZE;
	size_t sectors_to_erase = (sector_base_address_end - sector_base_address_start + 1);

	size_t bytes_to_erase = flash_address_end - flash_address_start + 1;	

	//--------------------------------------------------------------------------
	std::cout << "Erase memory:" << std::endl;
	std::cout << std::setbase(10) << sectors_to_erase << " sector(s) from #" << sector_num_start << " to #" << sector_num_end << std::endl;
	std::cout << bytes_to_erase << " byte(s) from " << std::setbase(16) << std::showbase << flash_address_start << " to " << flash_address_end;
	std::cout << "Erasing:" << std::endl;
	
	//--------------------------------------------------------------------------
	// Send write enable first
	spiflash_dev->write_enable();
	
	//--------------------------------------------------------------------------
	size_t cur_address = sector_base_address_start;
	for (size_t sector_num = sector_num_start; sector_num <= sector_num_end; ++sector_num)
	{
		try
		{
			std::cout << "Erasing sector # " << std::setbase(10) << sector_num;
			spiflash_dev->erase_sector(cur_address);
		}
		catch (...)
		{
			std::cout << " FAIL" << std::endl;
			throw SpiMemoryDriverError("erase error on sector #" + std::to_string(sector_num));
			return;
		}
		std::cout << " OK" << std::endl;
		
		cur_address += MEMORY_SECTOR_SIZE;
	}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t BaseSpiFlashMemoryDriver::read_id()
{
	return 0;
}