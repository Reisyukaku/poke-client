#pragma once

#include "hid.h"

namespace exl {
class Mouse {
public:
	static Mouse* getInstance() {
		if (instance == nullptr)
			instance = new Mouse();
		return instance;
	};
	
	void Poll()	{
		nn::hid::GetMouseState(&state);
	}

	void Update() {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent((float)state.x, (float)state.y);
		io.AddMouseButtonEvent(0, state.buttons.isBitSet(nn::hid::MouseButton::Left));
		io.AddMouseButtonEvent(1, state.buttons.isBitSet(nn::hid::MouseButton::Right));
	}
	
	
private:
	static Mouse* instance;
	nn::hid::MouseState state;
	
	Mouse() {
		nn::hid::InitializeMouse();
	}
	
	Mouse(const Mouse&);
	Mouse& operator=(const Mouse&);
};
};