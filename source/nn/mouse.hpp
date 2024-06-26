#pragma once

#include "hid.h"

constexpr int mouse_mapping[][2] = {
    {ImGuiMouseButton_Left,   static_cast<const int>(nn::hid::MouseButton::Left)},
    {ImGuiMouseButton_Right,  static_cast<const int>(nn::hid::MouseButton::Right)},
    {ImGuiMouseButton_Middle, static_cast<const int>(nn::hid::MouseButton::Middle)},
};

namespace pkcl {
class Mouse {
public:	
	void Poll()	{
		nn::hid::GetMouseState(&state);
	}

	void Update() {
		ImGuiIO &io = ImGui::GetIO();
		io.AddMousePosEvent((float)state.x, (float)state.y);
		for (auto [im_k, nx_k]: mouse_mapping)
		{
			io.AddMouseButtonEvent(im_k, state.buttons.isBitSet((nn::hid::MouseButton) nx_k));
		}
	}
	
	static Mouse* getInstance() {
		if (instance == nullptr)
			instance = new Mouse();
		return instance;
	};

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	
private:
	static Mouse* instance;
	nn::hid::MouseState state;
	
	Mouse() {
		nn::hid::InitializeMouse();
	}
};
};