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

private:
    static AboutForm* instance;
	AboutForm(const AboutForm&);
	AboutForm& operator=(const AboutForm&);

    AboutForm()
    {
        Name = "About";
    }

};