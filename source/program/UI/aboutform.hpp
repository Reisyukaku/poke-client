#pragma once

#include <string>
#include "imgui/imgui_nvn.h"
#include "imgui/imgui.h"
#include "form.hpp"

class AboutForm : public BasicForm {
public:
    
    void Draw() override;
    
    static AboutForm* getInstance() {
		if (instance == nullptr)
			instance = new AboutForm();
		return instance;
	};

    AboutForm(const AboutForm&) = delete;
	AboutForm& operator=(const AboutForm&) = delete;

private:
    static AboutForm* instance;

    AboutForm()
    {
        Name = "About";
    }

};