#pragma once

#include <vector>
#include <shtypes.h>
#include <string>

namespace oasis
{
	namespace abstractions
	{
		bool PickContainer(std::string& a_Path);
		bool PickFile(std::string& a_Path, const std::vector<COMDLG_FILTERSPEC>& a_Filters = {});
		bool SaveFile(const std::string& a_Path, int* a_Choice = nullptr, const std::vector<COMDLG_FILTERSPEC>& a_Filters = {});
		double Random(double a_Min, double a_Max);
	}
}