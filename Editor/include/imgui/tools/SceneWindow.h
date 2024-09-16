#pragma once

#include "imgui/tools/BaseWindow.h"

namespace oasis
{
	namespace imgui
	{
		class SceneWindow : public BaseWindow
		{
		public:
			SceneWindow();
			void Render() override;

			bool WindowBegin() override;
			void WindowEnd() override;
		};
		inline extern SceneWindow sceneWindow = {};
	}
}