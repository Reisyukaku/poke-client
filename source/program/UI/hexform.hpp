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
#include "imgui_memory_editor.h"

#ifdef _MSC_VER
#define _PRISizeT   "I"
#define ImSnprintf  _snprintf
#else
#define _PRISizeT   "z"
#define ImSnprintf  snprintf
#endif

class HexForm : public BasicForm{
public:
	
	void Initialize() override;
    void Draw() override;

    void SetAddr(uintptr_t addr) {
        DataAddr = addr;
    }

	static HexForm* getInstance() {
		if (instance == nullptr)
			instance = new HexForm();
		return instance;
	};
	
private:
	static HexForm* instance;
	HexForm(const HexForm&);
	HexForm& operator=(const HexForm&);

	static void WriteData(ImU8 *data, size_t off, ImU8 d);
	static ImU8 ReadData(const ImU8* data, size_t off);

	HexForm()
	{
		Name = "Hex Editor";
		DataAddr = 0;
		DataSize = 0x1000;

		keyboard = exl::Keyboard::getInstance();
	}

    uintptr_t DataAddr;
	size_t DataSize;
	exl::Keyboard *keyboard;
	static MemoryEditor *editor;
};