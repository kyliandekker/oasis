#pragma once

#include <string>

namespace oasis
{
	namespace imgui
	{
		// https://fontawesome.com/icons
		// https://r12a.github.io/app-conversion/
		// &#xf03d;
#define ICON_MIN_FA 0xe005
#define ICON_MAX_16_FA 0xf8ff
#define ICON_MAX_FA 0xf8ff
#define ICON_FA_CAMERA "\xEF\x80\xBD"
#define ICON_FA_LIGHT "\xEF\x83\xAB"
#define ICON_FA_LIGHT_ON "\xEF\x99\xB2"
#define ICON_FA_CUBE "\xEF\x86\xB2"
#define ICON_FA_BOX_COLLIDER "\xEF\x86\xB2"
#define ICON_FA_PLAYER "\xEF\x86\x83"
#define ICON_FA_CHICKEN "\xEF\x9F\xBB"
#define ICON_FA_SPHERE_COLLIDER "\xEF\x84\x91"
#define ICON_FA_MOVE "\xEF\x81\x87"
#define ICON_FA_ROTATE "\xEF\x8B\xB1"
#define ICON_FA_AUDIO "\xEF\x80\xA5"
#define ICON_FA_CHECKMARK_CHECKED "\xEF\x85\x8A"
#define ICON_FA_CHECKMARK "\xEF\x83\x88"
#define ICON_FA_COPY "\xEF\x83\x85"
#define ICON_FA_PASTE "\xEF\x83\xAA"
#define ICON_FA_RENAME "\xEF\x8C\x9C"
#define ICON_FA_DELETE "\xEF\x87\xB8"
#define ICON_FA_PARTICLE_SYSTEM "\xEF\x9D\xA2"
#define ICON_FA_FOLDED_IN "\xEF\x83\x9A"
#define ICON_FA_FOLDED_OUT "\xEF\x83\x97"
#define ICON_FA_ADD "\x2B"
#define ICON_FA_SEARCH "\xEF\x80\x82"
#define ICON_FA_ERASE "\xEF\x84\xAD"
#define ICON_FA_DOWN_TO_BOTTOM "\xEF\x8D\x8A"
#define ICON_FA_FOLDER "\xEF\x81\xBB"

#define ICON_FA_SCENE "\xEF\x80\x8A"
#define ICON_FA_HIERARCHY "\xEF\x80\xBA"
#define ICON_FA_INSPECTOR "\xEF\x81\x9A"
#define ICON_FA_CONSOLE "\xEE\x88\xB6"

#define ICON_FA_INFO "\xEF\x81\x9A"
#define ICON_FA_WARNING "\xEF\x81\xAA"
#define ICON_FA_ERROR "\xEF\x81\x97"
#define ICON_FA_ASSERT "\xEF\x9E\xBA"
#define ICON_FA_SUCCESS "\xEF\x81\x98"
#define ICON_FA_INFO_SUCCESS "\xEE\x84\x8D"
#define ICON_FA_AWESOME "\xEF\x93\x87"

#define IMGUI_INDENT 16.0f

		std::string IMGUI_FORMAT_ID(std::string a_Text, const char* a_ID, ...);

#define POPUP_WINDOW_ID "###POPUP_%s"
#define POPUP_WINDOW_BUTTON_ID "###POPUP_DIALOG_SAVE_BUTTON_%s"
#define CHILD_ID "###CHILD_%s"
#define BUTTON_ID "###BUTTON_%s"
#define WINDOW_ID "###WINDOW_%s"
#define MENU_ITEM_ID "###MENU_ITEM_%s"
#define TREE_NODE_ID "###TREE_NODE_%s"
#define MENU_ID "###MENU_%s"
#define INPUT_ID "###INPUT_%s"
#define CHECKBOX_ID "###CHECKBOX_%s"
#define FOLD_ID "###CHECKBOX_%s"
#define DRAG_ITEM_ID "###DRAG_ITEM_%s"
	}
}