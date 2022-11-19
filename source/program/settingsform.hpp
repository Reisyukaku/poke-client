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
    
    void Initialize(std::string name) override;
    void Draw() override;
    
};