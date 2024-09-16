#pragma once
#include <string>

namespace oasis
{
	namespace compiler
	{
		class Compiler
		{
		public:
			static bool Compile(const std::string& a_AssetDir);
		};
	}
}