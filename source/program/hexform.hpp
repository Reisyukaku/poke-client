#pragma once

#include <string>
#include <functional>
#include "imgui/imgui_nvn.h"
#include "nn/hid.h"
#include "offsetManager.hpp"
#include "nn/mouse.hpp"
#include "nn/keyboard.hpp"
#include "form.hpp"
#include "types.h"

#ifdef _MSC_VER
#define _PRISizeT   "I"
#define ImSnprintf  _snprintf
#else
#define _PRISizeT   "z"
#define ImSnprintf  snprintf
#endif

class HexForm : public BasicForm{
public:
	
    void Initialize(std::string name) override;
    void Draw() override;
    
	void DrawContents(void* mem_data_void, size_t mem_size);
    
	void SetCursorOffset(uint64_t offset) {
		Offset = offset;
	}
    
    void SetAddr(uintptr_t addr) {
        DataAddr = addr;
		snprintf(levelInput, IM_ARRAYSIZE(levelInput), "%016lX", DataAddr);
    }
	
private:
    uintptr_t DataAddr;
	uint64_t Offset;
	size_t DataSize;
    int Rows;
	bool OptGreyOutZeroes;
	int highlightMin, highlightMax;
	exl::Keyboard *keyboard;
	static char levelInput[32];
};