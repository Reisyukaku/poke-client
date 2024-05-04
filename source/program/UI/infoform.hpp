#pragma once

#include <string>
#include <deque>
#include "imgui/imgui_nvn.h"
#include "filelogger.hpp"
#include "nn/hid.h"
#include "nn/mouse.hpp"
#include "form.hpp"
#include "types.h"

class InfoForm : public BasicForm{
public:
    
    void Initialize() override;
    void Draw() override;
    
    void AddString(std::string s) {
        AddToQueue(s);
    }
	
    static InfoForm* getInstance() {
		if (instance == nullptr)
			instance = new InfoForm();
		return instance;
	};

private:
    static InfoForm* instance;
	InfoForm(const InfoForm&);
	InfoForm& operator=(const InfoForm&);

    InfoForm()
    {
        Name = "Info";
        isVisible = false;
        logger = exl::FileLogger::getInstance();
    }

    void AddToQueue(std::string str) {
        if(Logs.size() > 100)
            Logs.pop_front();
        Logs.push_back(str);
    }
    std::deque<std::string> Logs;
    exl::FileLogger *logger;
};