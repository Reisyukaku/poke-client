#pragma once

#include <string>
#include <deque>
#include "imgui/imgui_nvn.h"
#include "filelogger.hpp"
#include "tcplogger.hpp"
#include "nn/hid.h"
#include "nn/mouse.hpp"
#include "form.hpp"
#include "types.h"

class InfoForm : public BasicForm{
public:
    
    void Draw() override;
    
    void AddString(std::string s) {
        AddToQueue(s);
    }
	
    static InfoForm* getInstance() {
		if (instance == nullptr)
			instance = new InfoForm();
		return instance;
	};

    InfoForm(const InfoForm&) = delete;
	InfoForm& operator=(const InfoForm&) = delete;

private:
    static InfoForm* instance;

    InfoForm()
    {
        Name = "Info Log";
    }

    void AddToQueue(std::string str) {
        if(Logs.size() > 100)
            Logs.pop_front();
        Logs.push_back(str);
    }
    std::deque<std::string> Logs;
};