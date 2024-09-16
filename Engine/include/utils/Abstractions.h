#pragma once

#include <string>
#include <vector>
#include <shtypes.h>

namespace oasis
{
	namespace assets
	{
		enum class AssetType;
	}

	namespace abstractions
	{
		bool PickContainer(std::string& a_Path);
		bool PickFile(std::string& a_Path, const std::vector<COMDLG_FILTERSPEC>& a_Filters = {});
		bool SaveFile(std::string& a_Path, int* a_Choice = nullptr, const std::vector<COMDLG_FILTERSPEC>& a_Filters = {});
		bool SaveFolder(std::string& a_Path);
		std::vector<COMDLG_FILTERSPEC> GetFilters(assets::AssetType a_AssetType);
		double Random(double a_Min, double a_Max);
	}
}