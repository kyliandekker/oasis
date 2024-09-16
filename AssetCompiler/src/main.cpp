#include <iostream>
#include <compiler.h>

#include "ReserveDataStream.h"

int main(int argc, char* argv[], char* envp[])
{
	if (argc < 2)
	{
		std::cout <<
			"You have not provided an argument for compilation of decompilation." <<
			std::endl;
		return 0;
	}

	if (argc < 3)
	{
		std::cout <<
			"You have not provided a directory." <<
			std::endl;
		return 0;
	}

	if (strcmp(argv[1], "compile") == 0)
	{
		oasis::compiler::Compiler::Compile(argv[2]);
	}

	while (0)
	{ }

	return 0;
}