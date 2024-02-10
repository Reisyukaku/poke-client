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
	
	void Poll() {
		nn::hid::GetMouseState(&state);
	}
	
	nn::hid::MouseState state;
	
private:
	static Mouse* instance;
	
	Mouse() {
		nn::hid::InitializeMouse();
	}
	
	Mouse(const Mouse&);
	Mouse& operator=(const Mouse&);
};
};