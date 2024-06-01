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

	static void SetTheme(Themes theme)
	{
		ImGuiStyle *style = &ImGui::GetStyle();
		ImGui::StyleColorsDark(style);

		switch(theme)
		{
			case Green_Black:
			{
				style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.70f);

				style->Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    			style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.48f, 0.29f, 1.00f);

				style->Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.98f, 0.20f, 0.80f);
				style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.98f, 0.59f, 1.00f);
    			style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.98f, 0.53f, 1.00f);

				style->Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.98f, 0.59f, 1.00f);
				style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.98f, 0.59f, 0.35f);
				style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.69f, 0.47f, 0.40f);

				style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
				style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
				style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
				break;
			}
			default:
				break;
		}
	}
};