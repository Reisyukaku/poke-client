#pragma once

#include <string>
#include <deque>
#include "imgui/imgui_nvn.h"
#include "filelogger.hpp"
#include "tcplogger.hpp"
#include "nn/hid.h"
#include "nn/mouse.hpp"
#include "nvn/GfxSystem.hpp"
#include "form.hpp"
#include "types.h"

class InfoForm : public BasicForm{
public:
    
    void Draw() override;

    void SetNvnInfo(pkcl::NvnInfo info)
    {
        nvnInfo = info;
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
    pkcl::NvnInfo nvnInfo;
    
    InfoForm()
    {
        Name = "Info Log";
    }
};