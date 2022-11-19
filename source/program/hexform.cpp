#include "hexform.hpp"

void HexForm::Initialize(std::string name) {
	Name = name;
	isVisible = false;
	Rows = 16;
    DataSize = 0x1000;
    DataAddr = 0;
	OptGreyOutZeroes = false;
	highlightMin = -1;
	highlightMax = -1;
}
    
void HexForm::Draw() {
	bool draw_separator = true;
    float glyph_width = ImGui::CalcTextSize("F").x;
    float cell_width = glyph_width * 3; // "FF " we include trailing space in the width to easily catch clicks everywhere
    
    float wid = cell_width * Rows + 0x180;
	ImGui::SetNextWindowSize(ImVec2(wid, 500.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 0.0f), ImVec2(wid, FLT_MAX));

	if(!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar))
	{
        ImGui::End();
        return;
    }
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		ImGui::OpenPopup("context");
	
	
    
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

    int line_total_count = (int)((DataSize + Rows-1) / Rows);
	
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
	//Draw hex data
    for(int i = 0; i < line_total_count; i++){
        //Address
        ImGui::Text("%08lX: ", (uint64_t)(DataAddr + (i * Rows)));
        ImGui::SameLine(0, cell_width);
        
        //Data
		const ImU32 color_text = ImGui::GetColorU32(ImGuiCol_Text);
        const ImU32 color_disabled = OptGreyOutZeroes ? ImGui::GetColorU32(ImGuiCol_TextDisabled) : color_text;
		float startX = ImGui::GetCursorPosX();
        for(int j = 0; j < Rows; j++) {
			ImGui::PushStyleColor(ImGuiCol_Text, OptGreyOutZeroes ? color_disabled : color_text);
            ImGui::Text("%02X ", *(u8*)(DataAddr + (i * Rows) + j));
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{
				highlightMin = (i * Rows) + j;
			}
			if(((i * Rows) + j) == highlightMin){
				ImVec2 pos = ImGui::GetCursorScreenPos();
				float highlight_width = glyph_width * 2;
				float lineHeight = ImGui::GetTextLineHeight();
				draw_list->AddRectFilled(pos, ImVec2(pos.x + highlight_width, pos.y + lineHeight), IM_COL32(255, 255, 255, 50));
			}
            ImGui::SameLine();
			ImGui::PopStyleColor();
        }
        
		//Set position good bit after the data
		ImGui::SameLine(startX + cell_width * Rows + glyph_width * 2);
		
		//Only need to draw the divider once
		if(draw_separator){
			ImVec2 screen_pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddLine(ImVec2(screen_pos.x - glyph_width, screen_pos.y - 9999), ImVec2(screen_pos.x - glyph_width, screen_pos.y + 9999), ImColor(ImGui::GetStyle().Colors[ImGuiCol_Border]));
			draw_separator = false;
		}
        
        //Ascii
        for(int j = 0; j < Rows; j++) {
            char c = *(u8*)(DataAddr + (i * Rows) + j);
            char display_c = (c < 32 || c >= 128) ? '.' : c;
            ImGui::Text("%c", display_c);
            if(j < Rows - 1) ImGui::SameLine();
        }
    }
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
	
	ImGui::Separator();
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Range %08lX..%08lX", (uint64_t)DataAddr, (uint64_t)(DataAddr + DataSize-1));
	ImGui::SameLine();
	
	ImGui::End();
	
	if (ImGui::BeginPopup("context"))
	{
		ImGui::SetNextItemWidth(glyph_width * 7);
		ImGui::Checkbox("Grey out zeroes", &OptGreyOutZeroes);

		ImGui::EndPopup();
	}
}