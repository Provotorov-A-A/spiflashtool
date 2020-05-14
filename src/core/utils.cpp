//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <string>
#include <iostream>
#include <iomanip>
#include <cstddef>
#include <stdexcept>

//******************************************************************************
//								TYPES
//******************************************************************************

//******************************************************************************
//								VARIABLES
//******************************************************************************

//******************************************************************************
//								PROCEDURES	
//******************************************************************************
//------------------------------------------------------------------------------
/*	Вывод на экран size байт памяти: по 16 значений в строке, с адресами в начале
	каждой строки, начиная с адреса pMemStart. Параметр offset задаёт
	отображаемое значение первого адреса. Требуется в качестве параметра
	указатель на функцию, реализующую форматированный вывод (printf)*/
//------------------------------------------------------------------------------
void printMem(const uint8_t* const pMemStart, const size_t size, const size_t offset)
{
	int fExitFlag=0;

	size_t lines = size/16;
	if (size%16) lines+=1;
	
	std::cout << std::right << std::hex;
	for (size_t i=0;i<lines;i++)
	{
		std::cout << std::setw(10) << std::setfill(' ') << std::showbase << offset + i*16 << ":    ";
		for (size_t j=0; j<16; j++)
		{
			if (i*16+j >= size)
			{
				fExitFlag = 1;
				break;
			}
			std::cout << "  0x" << std::setw(2) << std::setfill('0') << std::noshowbase << (unsigned)(pMemStart[i*16+j]);
			if (j == 7)
			{
				std::cout << "  ";
			}
		}
		std::cout << std::endl;
		if(fExitFlag==1)
		{
			break;
		}
	}
	std::cout << std::endl;
	return;
}

//------------------------------------------------------------------------------
// Convert string to unsigned integer.
// Octal (prefix 0), decimal and hexical(prefix 0x or 0X) base are supported.
// Suffixes 'k', 'K', 'm', 'M', 'g', 'G' are supported
// Throws:
// std::invalid_argument, если преобразование не может быть выполнено
// std::out_of_range, если преобразованное значение будет выходить за границы диапазона значений типа результата
//------------------------------------------------------------------------------
size_t str_to_uint(const std::string s)
{
	std::string no_suffix;
	size_t mul = 1;
	size_t len = s.size();
	size_t res = 0;
	
	bool has_hex_prefix = (0 == s.substr(0,2).compare("0x")) || (0 == s.substr(0,2).compare("0X"));
	
	if ( (s.at(len-1) == 'k') || (s.at(len-1) == 'K') )
	{
		mul = 1024;
	}
	else if ( (s.at(len-1) == 'm') || (s.at(len-1) == 'M') )
	{
		mul = 1024*1024;
	}
	else if ( (s.at(len-1) == 'g') || (s.at(len-1) == 'G') )
	{
		mul = 1024*1024*1024;
	}
	else if ( has_hex_prefix && (!isxdigit(s.at(len-1))) )
	{
		throw(std::invalid_argument("bad prefix"));
	}
	
	if (1 != mul)
	{
		no_suffix = s.substr(0, len-1);
	}
	else
	{
		no_suffix = s;
	}
	
	size_t err_index = 0;
	res = stoull(no_suffix, &err_index, 0);
	if(err_index < no_suffix.size())
	{
		throw(std::invalid_argument("bad prefix"));
	};
	return 	(res*mul);
}