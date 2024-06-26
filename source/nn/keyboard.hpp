#pragma once

#include <map>
#include "imgui/imgui_nvn.h"
#include "imgui/imgui.h"
#include "nn/hid.h"
#include "hid.h"
#include "tcplogger.hpp"

constexpr int key_mapping[][2] = {
    {ImGuiKey_None,           0},
    {ImGuiKey_Tab,            static_cast<const int>(nn::hid::KeyboardKey::Tab)},
    {ImGuiKey_LeftArrow,      static_cast<const int>(nn::hid::KeyboardKey::LeftArrow)},
    {ImGuiKey_RightArrow,     static_cast<const int>(nn::hid::KeyboardKey::RightArrow)},
    {ImGuiKey_UpArrow,        static_cast<const int>(nn::hid::KeyboardKey::UpArrow)},
    {ImGuiKey_DownArrow,      static_cast<const int>(nn::hid::KeyboardKey::DownArrow)},
    {ImGuiKey_PageUp,         static_cast<const int>(nn::hid::KeyboardKey::PageUp)},
    {ImGuiKey_PageDown,       static_cast<const int>(nn::hid::KeyboardKey::PageDown)},
    {ImGuiKey_Home,           static_cast<const int>(nn::hid::KeyboardKey::Home)},
    {ImGuiKey_End,            static_cast<const int>(nn::hid::KeyboardKey::End)},
    {ImGuiKey_Insert,         static_cast<const int>(nn::hid::KeyboardKey::Insert)},
    {ImGuiKey_Delete,         static_cast<const int>(nn::hid::KeyboardKey::Delete)},
    {ImGuiKey_Backspace,      static_cast<const int>(nn::hid::KeyboardKey::Backspace)},
    {ImGuiKey_Space,          static_cast<const int>(nn::hid::KeyboardKey::Space)},
    {ImGuiKey_Enter,          static_cast<const int>(nn::hid::KeyboardKey::Return)},
    {ImGuiKey_Escape,         static_cast<const int>(nn::hid::KeyboardKey::Escape)},
    {ImGuiKey_LeftCtrl,       static_cast<const int>(nn::hid::KeyboardKey::LeftControl)},
    {ImGuiKey_LeftShift,      static_cast<const int>(nn::hid::KeyboardKey::LeftShift)},
    {ImGuiKey_LeftAlt,        static_cast<const int>(nn::hid::KeyboardKey::LeftAlt)},
    {ImGuiKey_LeftSuper,      static_cast<const int>(nn::hid::KeyboardKey::LeftGui)},
    {ImGuiKey_RightCtrl,      static_cast<const int>(nn::hid::KeyboardKey::RightControl)},
    {ImGuiKey_RightShift,     static_cast<const int>(nn::hid::KeyboardKey::RightShift)},
    {ImGuiKey_RightAlt,       static_cast<const int>(nn::hid::KeyboardKey::RightAlt)},
    {ImGuiKey_RightSuper,     static_cast<const int>(nn::hid::KeyboardKey::RightGui)},
    {ImGuiKey_Menu,           static_cast<const int>(nn::hid::KeyboardKey::Application)}, // not positive about this
    {ImGuiKey_0,              static_cast<const int>(nn::hid::KeyboardKey::D0)},
    {ImGuiKey_1,              static_cast<const int>(nn::hid::KeyboardKey::D1)},
    {ImGuiKey_2,              static_cast<const int>(nn::hid::KeyboardKey::D2)},
    {ImGuiKey_3,              static_cast<const int>(nn::hid::KeyboardKey::D3)},
    {ImGuiKey_4,              static_cast<const int>(nn::hid::KeyboardKey::D4)},
    {ImGuiKey_5,              static_cast<const int>(nn::hid::KeyboardKey::D5)},
    {ImGuiKey_6,              static_cast<const int>(nn::hid::KeyboardKey::D6)},
    {ImGuiKey_7,              static_cast<const int>(nn::hid::KeyboardKey::D7)},
    {ImGuiKey_8,              static_cast<const int>(nn::hid::KeyboardKey::D8)},
    {ImGuiKey_9,              static_cast<const int>(nn::hid::KeyboardKey::D9)},
    {ImGuiKey_A,              static_cast<const int>(nn::hid::KeyboardKey::A)},
    {ImGuiKey_B,              static_cast<const int>(nn::hid::KeyboardKey::B)},
    {ImGuiKey_C,              static_cast<const int>(nn::hid::KeyboardKey::C)},
    {ImGuiKey_D,              static_cast<const int>(nn::hid::KeyboardKey::D)},
    {ImGuiKey_E,              static_cast<const int>(nn::hid::KeyboardKey::E)},
    {ImGuiKey_F,              static_cast<const int>(nn::hid::KeyboardKey::F)},
    {ImGuiKey_G,              static_cast<const int>(nn::hid::KeyboardKey::G)},
    {ImGuiKey_H,              static_cast<const int>(nn::hid::KeyboardKey::H)},
    {ImGuiKey_I,              static_cast<const int>(nn::hid::KeyboardKey::I)},
    {ImGuiKey_J,              static_cast<const int>(nn::hid::KeyboardKey::J)},
    {ImGuiKey_K,              static_cast<const int>(nn::hid::KeyboardKey::K)},
    {ImGuiKey_L,              static_cast<const int>(nn::hid::KeyboardKey::L)},
    {ImGuiKey_M,              static_cast<const int>(nn::hid::KeyboardKey::M)},
    {ImGuiKey_N,              static_cast<const int>(nn::hid::KeyboardKey::N)},
    {ImGuiKey_O,              static_cast<const int>(nn::hid::KeyboardKey::O)},
    {ImGuiKey_P,              static_cast<const int>(nn::hid::KeyboardKey::P)},
    {ImGuiKey_Q,              static_cast<const int>(nn::hid::KeyboardKey::Q)},
    {ImGuiKey_R,              static_cast<const int>(nn::hid::KeyboardKey::R)},
    {ImGuiKey_S,              static_cast<const int>(nn::hid::KeyboardKey::S)},
    {ImGuiKey_T,              static_cast<const int>(nn::hid::KeyboardKey::T)},
    {ImGuiKey_U,              static_cast<const int>(nn::hid::KeyboardKey::U)},
    {ImGuiKey_V,              static_cast<const int>(nn::hid::KeyboardKey::V)},
    {ImGuiKey_W,              static_cast<const int>(nn::hid::KeyboardKey::W)},
    {ImGuiKey_X,              static_cast<const int>(nn::hid::KeyboardKey::X)},
    {ImGuiKey_Y,              static_cast<const int>(nn::hid::KeyboardKey::Y)},
    {ImGuiKey_Z,              static_cast<const int>(nn::hid::KeyboardKey::Z)},
    {ImGuiKey_F1,             static_cast<const int>(nn::hid::KeyboardKey::F1)},
    {ImGuiKey_F2,             static_cast<const int>(nn::hid::KeyboardKey::F2)},
    {ImGuiKey_F3,             static_cast<const int>(nn::hid::KeyboardKey::F3)},
    {ImGuiKey_F4,             static_cast<const int>(nn::hid::KeyboardKey::F4)},
    {ImGuiKey_F5,             static_cast<const int>(nn::hid::KeyboardKey::F5)},
    {ImGuiKey_F6,             static_cast<const int>(nn::hid::KeyboardKey::F6)},
    {ImGuiKey_F7,             static_cast<const int>(nn::hid::KeyboardKey::F7)},
    {ImGuiKey_F8,             static_cast<const int>(nn::hid::KeyboardKey::F8)},
    {ImGuiKey_F9,             static_cast<const int>(nn::hid::KeyboardKey::F9)},
    {ImGuiKey_F10,            static_cast<const int>(nn::hid::KeyboardKey::F10)},
    {ImGuiKey_F11,            static_cast<const int>(nn::hid::KeyboardKey::F11)},
    {ImGuiKey_F12,            static_cast<const int>(nn::hid::KeyboardKey::F12)},
    {ImGuiKey_Apostrophe,     static_cast<const int>(nn::hid::KeyboardKey::Quote)},
    {ImGuiKey_Comma,          static_cast<const int>(nn::hid::KeyboardKey::Comma)},
    {ImGuiKey_Minus,          static_cast<const int>(nn::hid::KeyboardKey::Minus)},
    {ImGuiKey_Period,         static_cast<const int>(nn::hid::KeyboardKey::Period)},
    {ImGuiKey_Slash,          static_cast<const int>(nn::hid::KeyboardKey::Slash)},
    {ImGuiKey_Semicolon,      static_cast<const int>(nn::hid::KeyboardKey::Semicolon)},
    {ImGuiKey_Equal,          static_cast<const int>(nn::hid::KeyboardKey::Plus)},
    {ImGuiKey_LeftBracket,    static_cast<const int>(nn::hid::KeyboardKey::OpenBracket)},
    {ImGuiKey_Backslash,      static_cast<const int>(nn::hid::KeyboardKey::Backslash)},
    {ImGuiKey_RightBracket,   static_cast<const int>(nn::hid::KeyboardKey::CloseBracket)},
    {ImGuiKey_GraveAccent,    static_cast<const int>(nn::hid::KeyboardKey::Backquote)},
    {ImGuiKey_CapsLock,       static_cast<const int>(nn::hid::KeyboardKey::CapsLock)},
    {ImGuiKey_ScrollLock,     static_cast<const int>(nn::hid::KeyboardKey::ScrollLock)},
    {ImGuiKey_NumLock,        static_cast<const int>(nn::hid::KeyboardKey::NumLock)},
    {ImGuiKey_PrintScreen,    static_cast<const int>(nn::hid::KeyboardKey::PrintScreen)},
    {ImGuiKey_Pause,          static_cast<const int>(nn::hid::KeyboardKey::Pause)},
    {ImGuiKey_Keypad0,        static_cast<const int>(nn::hid::KeyboardKey::NumPad0)},
    {ImGuiKey_Keypad1,        static_cast<const int>(nn::hid::KeyboardKey::NumPad1)},
    {ImGuiKey_Keypad2,        static_cast<const int>(nn::hid::KeyboardKey::NumPad2)},
    {ImGuiKey_Keypad3,        static_cast<const int>(nn::hid::KeyboardKey::NumPad3)},
    {ImGuiKey_Keypad4,        static_cast<const int>(nn::hid::KeyboardKey::NumPad4)},
    {ImGuiKey_Keypad5,        static_cast<const int>(nn::hid::KeyboardKey::NumPad5)},
    {ImGuiKey_Keypad6,        static_cast<const int>(nn::hid::KeyboardKey::NumPad6)},
    {ImGuiKey_Keypad7,        static_cast<const int>(nn::hid::KeyboardKey::NumPad7)},
    {ImGuiKey_Keypad8,        static_cast<const int>(nn::hid::KeyboardKey::NumPad8)},
    {ImGuiKey_Keypad9,        static_cast<const int>(nn::hid::KeyboardKey::NumPad9)},
    {ImGuiKey_KeypadDecimal,  static_cast<const int>(nn::hid::KeyboardKey::NumPadDot)},
    {ImGuiKey_KeypadDivide,   static_cast<const int>(nn::hid::KeyboardKey::NumPadDivide)},
    {ImGuiKey_KeypadMultiply, static_cast<const int>(nn::hid::KeyboardKey::NumPadMultiply)},
    {ImGuiKey_KeypadSubtract, static_cast<const int>(nn::hid::KeyboardKey::NumPadSubtract)},
    {ImGuiKey_KeypadAdd,      static_cast<const int>(nn::hid::KeyboardKey::NumPadAdd)},
    {ImGuiKey_KeypadEnter,    static_cast<const int>(nn::hid::KeyboardKey::NumPadEnter)},
    {ImGuiKey_KeypadEqual,    static_cast<const int>(nn::hid::KeyboardKey::NumPadEquals)},
};

namespace pkcl 
{
class Keyboard 
{
public:	
	void Poll() 
    {
        prevState = state;
		nn::hid::GetKeyboardState(&state);
	}
    
    void Update() 
    {
        for (auto [im_k, nx_k]: key_mapping) {
            if (IsPressed((nn::hid::KeyboardKey) nx_k))
                ImGui_ImplNX_AddKeyEvent((ImGuiKey) im_k, true);
            else if(IsReleased((nn::hid::KeyboardKey) nx_k))
                ImGui_ImplNX_AddKeyEvent((ImGuiKey) im_k, false);
        }
    }
    
    static Keyboard* getInstance() 
    {
		if (instance == nullptr)
			instance = new Keyboard();
		return instance;
	};

    Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	
private:
	static Keyboard* instance;

    nn::hid::KeyboardState state;
	nn::hid::KeyboardState prevState;
    std::map<u32, char> keyMap;
    
	Keyboard() 
    {
		nn::hid::InitializeKeyboard();
	}

    bool IsPressed(nn::hid::KeyboardKey nx_k)
    {
        return state.keys.isBitSet(nx_k) && !prevState.keys.isBitSet(nx_k);
    }

    bool IsReleased(nn::hid::KeyboardKey nx_k)
    {
        return !state.keys.isBitSet(nx_k) && prevState.keys.isBitSet(nx_k);
    }

    void ImGui_ImplNX_AddKeyEvent(ImGuiKey key, bool down)
    {
        ImGuiIO &io = ImGui::GetIO();
        //Add mod events from keys
        if(key == ImGuiKey_LeftShift || key == ImGuiKey_RightShift)
            io.AddKeyEvent(ImGuiMod_Shift, down);
        if(key == ImGuiKey_LeftCtrl || key == ImGuiKey_RightCtrl)
            io.AddKeyEvent(ImGuiMod_Ctrl, down);
        if(key == ImGuiKey_LeftAlt || key == ImGuiKey_RightAlt)
            io.AddKeyEvent(ImGuiMod_Alt, down);
        
        //Add other key events
        io.AddKeyEvent(key, down);

        if(!down) return;

        //Add char to input buffer if applicable
        if(key >= ImGuiKey_0 && key <= ImGuiKey_9)
        {
            if(ImGui::IsKeyDown(ImGuiKey_ModShift))
                io.AddInputCharacter(")!@#$%^&*("[(key - ImGuiKey_0)]);
            else
                io.AddInputCharacter("0123456789"[(key - ImGuiKey_0)]);
        }
        if(key >= ImGuiKey_Apostrophe && key <= ImGuiKey_RightBracket)
        {
            if(ImGui::IsKeyDown(ImGuiKey_ModShift))
                io.AddInputCharacter("\"<_>?:+{|}"[(key - ImGuiKey_Apostrophe)]);
            else
                io.AddInputCharacter("',-./;=[\\]"[(key - ImGuiKey_Apostrophe)]);
        }
        if(key >= ImGuiKey_A && key <= ImGuiKey_Z)
        {
            io.AddInputCharacter((ImGui::IsKeyDown(ImGuiKey_ModShift) ? 'A' : 'a') + (key - ImGuiKey_A));
        }
        if(key == ImGuiKey_Space)
            io.AddInputCharacter(' ');

        if(key == ImGuiKey_Tab)
            io.AddInputCharacter('\t');
    }
};
};