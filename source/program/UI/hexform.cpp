#include "hexform.hpp"

HexForm * HexForm::instance = nullptr;
MemoryEditor *HexForm::editor = nullptr;

void HexForm::Initialize()
{
	editor = new MemoryEditor();
}
    
void HexForm::Draw() {
	float glyph_width = ImGui::CalcTextSize("F").x;
    float cell_width = glyph_width * 3; // "FF " we include trailing space in the width to easily catch clicks everywhere
    
    float wid = cell_width * editor->Cols + 0x180;
	ImGui::SetNextWindowSize(ImVec2(wid, 500.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 0.0f), ImVec2(wid, FLT_MAX));

	if(!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar))
	{
        ImGui::End();
        return;
    }
	editor->DrawContents((void*)DataAddr, 0x1000, (size_t)DataAddr);
	ImGui::End();
}