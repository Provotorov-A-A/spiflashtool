//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <iostream>
#include <algorithm>
#include <stdio.h>

#include "core/include/arg_parser.h"
#include "core/include/serial_interface.h"
#include "core/include/progdevice.h"
#include "core/include/global_opts.h"
#include "target_specific/include/win_serial.h"
#include "core/include/base_spi_mem_dev.h"
#include "core/include/spiflash_mem_driver_base.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;

//******************************************************************************
//								VARIABLES
//******************************************************************************
GlobalOptions global_opts;

static const std::string help_str= std::string
(
"Usage: spiflashtool [operation] <key1> <value1> <key2> <value2> ...\n" \
"operation: -read, -write, -read_id, -erase, -help\n" \
"Keys:\n" \
"  -address        start address in flash memory for specified operation. Default value = 0.\n" \
"  -size           data size in bytes that will be read/write/erased. Default value = 0. For write operation 0 means \"all file\"\n" \
"  -filename       data source for write operation and data destination for read operation\n" \
"  -crc            switch on CRC16 calculating for every data packet\n" \
"  -verbose        full process listing\n" \
"  -port           serial port name\n" \
"  -baudrate       serial baudrate\n"
);

//******************************************************************************
//								PROCEDURES
//******************************************************************************
static void help_print()
{
	std::cout << std::endl << help_str;
}

void printMem(int (*printf) (const char*	fmt, ...), const uint8_t* pMemStart, const uint16_t size, const uint32_t offset);

//******************************************************************************
//								ENTRY
//******************************************************************************
int main(int argc, char *argv[])
{
	// Command line arguments parsing
	try
	{
		parse(argc, argv);
	}
	catch (ArgParserException& e)
	{
		((CommonException&)e).print();
		exit(-1);
	}
	catch(...)
	{
		std::cout << "UNKNOWN EXCEPTION RAISED!" << std::endl;
		exit(-1);
	}

	// cout << "==================================================================" << endl;
	// global_opts.print();
	// cout << "==================================================================" << endl;


//----------------------------------------------
// DEBUG
	
	try 
	{
		std::cout << "Create Serial ... ";
		WindowsSerial serial(global_opts.hardware_if_opts);
		std::cout << "OK" << std::endl;
		
		std::cout << "Create ProgDevice ... ";
		ProgDevice pd((SerialInterface*)&serial);
		std::cout << "OK" << std::endl;
		
		std::cout << "Create BaseSpiMemoryDevice ... ";
		BaseSpiMemoryDevice mem(BaseSpiMemoryDevice(&pd, 1000000));
		std::cout << "OK" << std::endl;

		std::cout << "Create BaseSpiFlashMemoryDriver ... ";
		BaseSpiFlashMemoryDriver mem_drv(&mem);
		std::cout << "OK" << std::endl;

		
// DEBUG END
//----------------------------------------------
		cout << "==================================================================" << endl;
		global_opts.print();
		cout << "==================================================================" << endl;

		// Perform specified operation
		
		switch (global_opts.operation)
		{
			case TOOL_OPERATION_HELP_PRINT:
			{
				help_print();
			}
			break;

			case TOOL_OPERATION_READ_JEDEC_ID:
			{
				std::cout << "Read id ... ";
				uint32_t id = mem.read_id();
				std::cout << "OK, id = " << std::hex << id << std::endl;
			}
			break;

			case TOOL_OPERATION_READ_MEM:
			{
				std::cout << "Read data ... ";
				uint32_t address = global_opts.progdev_opts.address;
				size_t size = global_opts.progdev_opts.size;

				mem_drv.read_mem_to_file(global_opts.progdev_opts.filename, address, size);
				
				std::cout << "OK" << std::endl;
			}
			break;

			case TOOL_OPERATION_ERASE_MEM:
			{
				std::cout << "Erase data ... ";
				uint32_t address = global_opts.progdev_opts.address;
				size_t size = global_opts.progdev_opts.size;
				mem_drv.erase(address, size);
				std::cout << "OK" << std::endl;
			}
			break;
			
			case TOOL_OPERATION_WRITE_MEM:
			{
				std::cout << "Program data ... ";
				uint32_t address = global_opts.progdev_opts.address;
				size_t size = global_opts.progdev_opts.size;
				
				mem_drv.write_mem_from_file(global_opts.progdev_opts.filename, address, size);
				std::cout << "OK" << std::endl;
			}
			break;
			
			default:
			{
				std::cout << "No operation was specified. Exit." << std::endl;
			}
			break;
		}
	}
	catch (const CommonException& e)
	{
		e.print();
		exit(-1);
	}
	// catch (ArgParserException& e)
	// {
		// ((CommonException&)e).print();
		// exit(-1);
	// }
	// catch (HW_IO_Error& e)
	// {
		// ((CommonException&)e).print();
		// exit(-1);
	// }
	// catch (ProgDevice_Error& e)
	// {
		// ((CommonException&)e).print();
		// exit(-1);
	// }	
	// catch (SpiMemoryError& e)
	// {
		// ((CommonException&)e).print();
		// exit(-1);
	// }
	catch(...)
	{
		std::cout << "UNKNOWN EXCEPTION RAISED!" << std::endl;
		exit(-1);
	}	
	
	std::cout << "FINISH" << std::endl;
	return 0;
}




//------------------------------------------------------------------------------
/*	Вывод на экран size байт памяти: по 16 значений в строке, с адресами в начале
	каждой строки, начиная с адреса pMemStart. Параметр offset задаёт
	отображаемое значение первого адреса. Требуется в качестве параметра
	указатель на функцию, реализующую форматированный вывод (printf)*/
//------------------------------------------------------------------------------
void printMem(int (*printf) (const char*	fmt, ...), const uint8_t* pMemStart, const uint16_t size, const uint32_t offset)
{
	printf("\n\r");
	int fExitFlag=0;

	uint16_t lines = size/16;
	if (size%16) lines+=1;

	for (uint16_t i=0;i<lines;i++)
	{
		printf("0x%08X    ",offset + i*16);
		for (uint16_t j=0;j<16;j++)
		{
			if (i*16+j >= size)
			{
				fExitFlag = 1;
				break;
			}
			printf("0x%02X ",pMemStart[i*16+j]);
			if (j == 7)
			{
				printf("  ");
			}
			/*задержка, чтобы printf не захлебнулся*/
//			for(volatile uint32_t i=0;i<10000;i++);
		}
		printf("\n\r");
		if(fExitFlag==1)
			break;
	}
	printf("\n\r");
	return;
}
