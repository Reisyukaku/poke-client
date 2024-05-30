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
    
    static SettingsForm* getInstance() {
		if (instance == nullptr)
			instance = new SettingsForm();
		return instance;
	};

private:
    static SettingsForm* instance;
	SettingsForm(const SettingsForm&);
	SettingsForm& operator=(const SettingsForm&);

    SettingsForm()
    {
        Name = "Settings";
    }

};