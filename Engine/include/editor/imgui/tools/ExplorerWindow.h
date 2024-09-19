#pragma once

#include "imgui/tools/BaseWindow.h"

namespace oasis
{
	namespace imgui
	{
		class ExplorerWindow : public BaseWindow
		{
		public:
			ExplorerWindow();
			void Render() override;
		};
		inline extern ExplorerWindow explorerWindow = {};
	}
}