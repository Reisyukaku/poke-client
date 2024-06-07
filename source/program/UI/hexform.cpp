#include "hexform.hpp"

HexForm * HexForm::instance = nullptr;
MemoryEditor *HexForm::editor = nullptr;

void HexForm::WriteData(ImU8 *data, size_t off, ImU8 d)
{
	data[off] = d;
}

ImU8 HexForm::ReadData(const ImU8* data, size_t off)
{
	return data[off];
}
    
void HexForm::Draw() {
	editor->DrawWindow(Name.c_str(), &isVisible, (void*)DataAddr, DataSize, DataAddr);
}