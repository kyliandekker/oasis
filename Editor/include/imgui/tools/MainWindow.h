#pragma once

#include "imgui/tools/BaseWindow.h"

namespace oasis
{
	namespace imgui
	{
		class MainWindow : public BaseWindow
		{
		public:
			MainWindow();
			void Render() override;
		};
	}
}