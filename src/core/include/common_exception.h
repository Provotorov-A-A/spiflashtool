#ifndef  _COMMON_EXCEPTION_H_
#define _COMMON_EXCEPTION_H_

//******************************************************************************
//								INCLUDES
//******************************************************************************
#include <exception>
#include <iostream>
#include <string>

//******************************************************************************
//								TYPES
//******************************************************************************
class CommonException
{
private:

protected:
	std::string module_name;
	std::string description;
	
public:	
	CommonException(const std::string& module, const std::string& d) : module_name(module), description(d)
	{	
	};
	
	const char* what() const 
	{
		return (module_name + " : " + description).c_str();
	};
};

#endif /* _COMMON_EXCEPTION_H_ */