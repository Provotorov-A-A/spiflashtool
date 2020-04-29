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
-help				|	print help.

### Operation options.
Operation options might have one argument(-address 0x1000) or have no argument (-verbose)
operation options:
Command				|	Description
------------------- |---------------------------------
-address			|	spi flash memory address where read/write will be starts. Argument is an integer in hexadecimal format (0x012ABCDEF).
-size				|	size of data that will be read/writen (in bytes). Argument is an integer in decimal format (1024). No size option in write operation means program all data from file.
-file 				|	data destibation/source file name.
-verbose			|	print all intermidiate data.

### Interface options:
Command				|	Description
------------------- |---------------------------------
-port				|	name of interface port (supported only serial interfaces). Windows example: -p COM3.
-baudrate			|	interface baudrate. Argument is an integer in decimal format (supported only standard baudrates (4800, 9600, ..., 115200). 


### Examples
*Read Jedec id*
`<spi_flash_burner -read_id -port COM15 -baudrate 57600>`

*Read memory to file*
`<spi_flash_burner -read -port COM15 -baudrate 115200 -address 0x00 -size 512 -file "c:\work\flash.img" -verbose>`

*Write data from file to memory*
`<spi_flash_burner -write -port COM15 -baudrate 115200 -address 0x00 -size 2048 -file "c:\work\flash.img" -verbose>`
