//******************************************************************************
//								INCLUDES
//******************************************************************************

#include <iostream>
#include <algorithm>

#include "core/include/arg_parser.h"
#include "core/include/serial.h"
#include "core/include/progdevice.h"
#include "core/include/global_opts.h"

//******************************************************************************
//								TYPES
//******************************************************************************
using namespace std;

//******************************************************************************
//								VARIABLES
//******************************************************************************
GlobalOptions global_opts;
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
	}
	catch(...)
	{
		cout << "UNKNOWN EXCEPTION RAISED!" << std::endl;
	}

	

	
	cout << "==================================================================" << endl;
	global_opts.print();
	cout << "==================================================================" << endl;
		
	return 0;
}