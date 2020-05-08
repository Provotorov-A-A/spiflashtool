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
	static const std::string common_name;
	
	CommonException(const std::string d);

protected:
	std::string module_name;
	std::string description;
	
public:	
	CommonException(const std::string module, const std::string d);

	const char* what() const;
	void print() const;
	
};

#endif /* _COMMON_EXCEPTION_H_ */