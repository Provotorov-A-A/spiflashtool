#ifndef  _COMMON_EXCEPTION_H_
#define _COMMON_EXCEPTION_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <iostream>
#include <string>

//******************************************************************************
//								TYPES
//******************************************************************************

class CommonException
{
protected:
	std::string module_name;
	std::string description;
public:	
	CommonException(std::string module, std::string d) : module_name(module), description(d) {};

	const char* what() const 
	{
		return description.c_str();
	};
	
	void print() const
	{
		if (0 == description.size())
		{
			std::cout << std::endl << "Unknown exception occured in module \"" << module_name << "\" : " << std::endl;
		}
		else
		{
			std::cout << std::endl << "Exception occured in module \"" << module_name << "\" : "<< description << std::endl;
		}
	}
};

#endif /* _COMMON_EXCEPTION_H_ */