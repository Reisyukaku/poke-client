#pragma once

#include <string>
#include <functional>
#include "imgui/imgui_nvn.h"
#include "nn/hid.h"
#include "nn/mouse.hpp"
#include "types.h"

class BasicForm {
public:
    
    virtual void Initialize(std::string name);
    virtual void Draw();
	
	void Open() {
		isVisible = !isVisible;
	}
	
	bool IsOpen() {
		return isVisible;
	}
	
protected:
	std::string Name;
	bool isVisible;
};