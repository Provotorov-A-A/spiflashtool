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
private:
	std::string module_name;
	std::string description;
public:	
	CommonException(std::string module, std::string d) : module_name(module), description(d) {};
	void print()
	{
		std::cout << "Error occured in module \"" << module_name << "\" :"<< std::endl << description << std::endl;
	}
};

#endif /* _COMMON_EXCEPTION_H_ */