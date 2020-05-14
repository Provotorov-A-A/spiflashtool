# Description.
PC-side software of spi flash programmer.
Command-line tool for read, program and erase spi flash memory.

## Command line interface.
spi_flash_burner <operation> <operation options> <interface options> 

### Operation commands.
Operation commands have no options.

Command				|	Description
------------------- |---------------------------------
-read_id			| 	read JEDEC ID of memory device. Id will be print in stdout.
-read				|	read memory to file.
-write				|	program memory to file.
-erase				|	erase (full chip only).
-link				|	test connection with prog. device
-jump				|	prog. device performs jump to specified address (auxiliary function for device testing)
-help				|	print help.

### Operation options.
Operation options might have one argument(-address 0x1000) or have no argument (-verbose)
operation options:
Command				|	Description
------------------- |---------------------------------
-address			|	spi flash memory address where read/write will be starts. Argument is an integer in decimal or hexadecimal format. Suffixes 'k', 'm' and 'g' are supported.
-size				|	size of data that will be read/writen (in bytes). Argument is an integer in decimal or hex format. Suffixes 'k', 'm' and 'g' are supported. No size option in write operation means program all data from file.
-filename 				|	data destination/source file name.
-verbose			|	print configuration and intermidiate data if possible.
-m				|	select memory device and it's driver that will be used to operate. 'base' is the most common configuration (and is only avalaible just now:)). If no -m option was specified than 'base' configuration will be applied. 
-crc    | IN WORK! Add CRC for prog. device protocol.

### Interface options:
Command				|	Description
------------------- |---------------------------------
-port				|	name of interface port (supported only serial interfaces). Windows example: -p COM3.
-baudrate			|	IN WORK!. interface baudrate. Argument is an integer in decimal format (supported only standard baudrates (4800, 9600, ..., 115200). 


### Examples
*Read Jedec id*
`<spi_flash_burner -read_id -port COM15 -baudrate 57600>`

*Read memory to file*
`<spi_flash_burner -m base -read -port COM15 -baudrate 115200 -address 32K -size 2M -filename "c:\work\flash.img" >`

*Write data from file to memory*
`<spi_flash_burner -m base -write -port COM15 -baudrate 115200 -address 0x1000 -filename "c:\work\flash.img" -verbose>`
