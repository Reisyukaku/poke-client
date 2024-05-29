#include "hexform.hpp"

HexForm * HexForm::instance = nullptr;
MemoryEditor *HexForm::editor = nullptr;

void HexForm::WriteData(ImU8 *data, size_t off, ImU8 d)
{
	data[off] = d; //TODO check memory perms
}

ImU8 HexForm::ReadData(const ImU8* data, size_t off)
{
	return data[off]; //TODO check memory perms
}

void HexForm::Initialize()
{
	editor = new MemoryEditor();
	editor->WriteFn = HexForm::WriteData;
	editor->ReadFn = HexForm::ReadData;
}
    
void HexForm::Draw() {
	editor->DrawWindow(Name.c_str(), (void*)DataAddr, DataSize, DataAddr);
}