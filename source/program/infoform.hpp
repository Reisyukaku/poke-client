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
    
    void Initialize(std::string name) override;
    void Draw() override;
    
    void AddString(std::string s) {
        AddToQueue(s);
    }
    
    void AddLogs(std::vector<std::string> logs) {
        for(auto l : logs){
            AddToQueue(l);
        }
    }
	
private:
    void AddToQueue(std::string str) {
        if(Logs.size() > 100)
            Logs.pop_front();
        Logs.push_back(str);
    }
    std::deque<std::string> Logs;
    exl::FileLogger *logger;
};