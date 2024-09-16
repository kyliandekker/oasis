#pragma once

#include "imgui/tools/BaseWindow.h"

namespace oasis
{
	namespace imgui
	{
		class InspectorWindow : public BaseWindow
		{
		public:
			InspectorWindow();
			void Render() override;

			bool WindowBegin() override;
			void WindowEnd() override;
		};
		inline extern InspectorWindow inspectorWindow = {};
	}
}