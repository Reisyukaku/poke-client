#pragma once

#include <string>
#include "imgui/imgui_nvn.h"
#include "nn/hid.h"
#include "form.hpp"
#include "types.h"

class ThemeManager {
public:
	enum Themes {
		Default,
		Green_Black
	};

	void SetTheme(Themes theme)
	{
		ImGuiStyle *style = &ImGui::GetStyle();
		ImGui::StyleColorsDark(style);

		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.74f);
		switch(theme)
		{
			case Default:
			{
				break;
			}
			case Green_Black:
			{
				style->Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.98f, 0.20f, 0.80f);
				break;
			}
		}
	}

	static ThemeManager* getInstance() {
		if (instance == nullptr)
			instance = new ThemeManager();
		return instance;
	};
	
private:
	static ThemeManager* instance;
	ThemeManager(const ThemeManager&);
	ThemeManager& operator=(const ThemeManager&);

	ThemeManager()
	{
        //
	}
};