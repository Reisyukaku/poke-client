#pragma once

#include <string>
#include <functional>
#include "imgui/imgui_nvn.h"
#include "filelogger.hpp"
#include "nn/hid.h"
#include "nn/mouse.hpp"
#include "form.hpp"
#include "types.h"

class SettingsForm : public BasicForm{
public:
    
    void Draw() override;

    bool WireframeEnabled()
    {
        return wireframeMode;
    }
    
    static SettingsForm* getInstance() {
		if (instance == nullptr)
			instance = new SettingsForm();
		return instance;
	};

    SettingsForm(const SettingsForm&) = delete;
	SettingsForm& operator=(const SettingsForm&) = delete;


protected:
    bool wireframeMode;

private:
    static SettingsForm* instance;

    SettingsForm()
    {
        Name = "Settings";
    }

};