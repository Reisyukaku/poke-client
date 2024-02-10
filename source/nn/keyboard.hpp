#pragma once

#include <map>
#include "imgui/imgui_nvn.h"
#include "imgui/font.h"
#include "hid.h"

namespace exl {
class Keyboard {
public:
	static Keyboard* getInstance() {
		if (instance == nullptr)
			instance = new Keyboard();
		return instance;
	};
	
	void Poll() {
        prevState = state;
		nn::hid::GetKeyboardState(&state);
	}
    
    void AddKeys() {
        ImGuiIO& io = ImGui::GetIO();
        auto keys = ParseKeystate();
        for(auto &k : keys)
            io.AddInputCharacter(k);
    }
    
    s64 GetSampleNum(){
        return state.sampleNum;
    }
    
    void SetMap() {
        ImGuiIO &io = ImGui::GetIO();
        io.KeyMap[ImGuiKey_Backspace] = 0x2A;
        io.KeyMap[ImGuiKey_Escape] = 0x29;
        io.KeyMap[ImGuiKey_Enter] = 0x28;
        io.KeyMap[ImGuiKey_Tab] = 0x2B;
        io.KeyMap[ImGuiKey_RightArrow] = 0x4F;
        io.KeyMap[ImGuiKey_LeftArrow] = 0x50;
        io.KeyMap[ImGuiKey_DownArrow] = 0x51;
        io.KeyMap[ImGuiKey_UpArrow] = 0x52;
        keyMap[0x04] =  'A';                 
        keyMap[0x05] =  'B';                 
        keyMap[0x06] =  'C';                 
        keyMap[0x07] =  'D';                 
        keyMap[0x08] =  'E';                 
        keyMap[0x09] =  'F';                 
        keyMap[0x0A] =  'G';                 
        keyMap[0x0B] =  'H';                 
        keyMap[0x0C] =  'I';                 
        keyMap[0x0D] =  'J';                 
        keyMap[0x0E] =  'K';                 
        keyMap[0x0F] =  'L';                 
        keyMap[0x10] =  'M';                 
        keyMap[0x11] =  'N';                 
        keyMap[0x12] =  'O';                 
        keyMap[0x13] =  'P';                 
        keyMap[0x14] =  'Q';                 
        keyMap[0x15] =  'R';                 
        keyMap[0x16] =  'S';                 
        keyMap[0x17] =  'T';                 
        keyMap[0x18] =  'U';                 
        keyMap[0x19] =  'V';                 
        keyMap[0x1A] =  'W';                 
        keyMap[0x1B] =  'X';                 
        keyMap[0x1C] =  'Y';                 
        keyMap[0x1D] =  'Z';                 
        keyMap[0x1E] =  '1';                
        keyMap[0x1F] =  '2';                
        keyMap[0x20] =  '3';                
        keyMap[0x21] =  '4';                
        keyMap[0x22] =  '5';                
        keyMap[0x23] =  '6';                
        keyMap[0x24] =  '7';                
        keyMap[0x25] =  '8';                
        keyMap[0x26] =  '9';                
        keyMap[0x27] =  '0';                           
        keyMap[0x2C] =  ' ';              
        keyMap[0x2D] =  '-';            
        keyMap[0x2E] =  '+';             
        keyMap[0x2F] =  '[';      
        keyMap[0x30] =  ']';      
        keyMap[0x31] =  '|';             
        keyMap[0x32] =  '~';             
        keyMap[0x33] =  ';';         
        keyMap[0x34] =  '\'';             
        keyMap[0x35] =  '`';         
        keyMap[0x36] =  ',';             
        keyMap[0x37] =  '.';            
        keyMap[0x38] =  '/';             
        //keyMap[0x39] =  CapsLock;          
        /*keyMap[0x3A] =  F1;                
        keyMap[0x3B] =  F2;                
        keyMap[0x3C] =  F3;                
        keyMap[0x3D] =  F4;                
        keyMap[0x3E] =  F5;                
        keyMap[0x3F] =  F6;                
        keyMap[0x40] =  F7;                
        keyMap[0x41] =  F8;                
        keyMap[0x42] =  F9;                
        keyMap[0x43] =  F10;               
        keyMap[0x44] =  F11;               
        keyMap[0x45] =  F12;               
        keyMap[0x46] =  PrintScreen;       
        keyMap[0x47] =  ScrollLock;        
        keyMap[0x48] =  Pause;             
        keyMap[0x49] =  Insert;            
        keyMap[0x4A] =  Home;              
        keyMap[0x4B] =  PageUp;            
        keyMap[0x4C] =  Delete;            
        keyMap[0x4D] =  End;   
        keyMap[0x4E] =  PageDown;          
        keyMap[0x4F] =  RightArrow;        
        keyMap[0x50] =  LeftArrow;         
        keyMap[0x51] =  DownArrow;         
        keyMap[0x52] =  UpArrow;           
        keyMap[0x53] =  NumLock;           
        keyMap[0x54] =  NumPadDivide;      
        keyMap[0x55] =  NumPadMultiply;    
        keyMap[0x56] =  NumPadSubtract;    
        keyMap[0x57] =  NumPadAdd;         
        keyMap[0x58] =  NumPadEnter; */      
        keyMap[0x59] =  '1';           
        keyMap[0x5A] =  '2';           
        keyMap[0x5B] =  '3';           
        keyMap[0x5C] =  '4';           
        keyMap[0x5D] =  '5';           
        keyMap[0x5E] =  '6';           
        keyMap[0x5F] =  '7';           
        keyMap[0x60] =  '8';           
        keyMap[0x61] =  '9';           
        keyMap[0x62] =  '0';           
        keyMap[0x63] =  '.';         
        /*keyMap[0x64] =  Backslash;         
        keyMap[0x65] =  Application;       
        keyMap[0x66] =  Power;             
        keyMap[0x67] =  NumPadEquals;      
        keyMap[0x68] =  F13;               
        keyMap[0x69] =  F14;               
        keyMap[0x6A] =  F15;               
        keyMap[0x6B] =  F16;               
        keyMap[0x6C] =  F17;               
        keyMap[0x6D] =  F18;               
        keyMap[0x6E] =  F19;               
        keyMap[0x6F] =  F20;               
        keyMap[0x70] =  F21;               
        keyMap[0x71] =  F22;               
        keyMap[0x72] =  F23;               
        keyMap[0x73] =  F24;               
        keyMap[0x85] =  NumPadComma;       
        keyMap[0x87] =  Ro;                
        keyMap[0x88] =  KatakanaHiragana;  
        keyMap[0x89] =  Yen;               
        keyMap[0x8A] =  Henkan;            
        keyMap[0x8B] =  Muhenkan;          
        keyMap[0x8C] =  NumPadCommaPc98;   
        keyMap[0x90] =  HangulEnglish;     
        keyMap[0x91] =  Hanja;             
        keyMap[0x92] =  Katakana;          
        keyMap[0x93] =  Hiragana;          
        keyMap[0x94] =  ZenkakuHankaku;    
        keyMap[0xE0] =  LeftControl;       
        keyMap[0xE1] =  LeftShift;         
        keyMap[0xE2] =  LeftAlt;           
        keyMap[0xE3] =  LeftGui;           
        keyMap[0xE4] =  RightControl;      
        keyMap[0xE5] =  RightShift;        
        keyMap[0xE6] =  RightAlt;          
        keyMap[0xE7] =  RightGui; */
    }
	
private:
	static Keyboard* instance;
    nn::hid::KeyboardState state;
	nn::hid::KeyboardState prevState;
    std::map<u32, char> keyMap;
    
	Keyboard() {
		nn::hid::InitializeKeyboard();
	}
    
    std::vector<u32> ParseKeystate() {
        std::vector<u32> codes;
        bool isShift = (state.Keys[0xE1/64] >> (0xE1 % 64)) & 1; //get shift key
        for(int i = 0; i < 0xFF; i++){
            if(i == 0xE1) continue; //skip shift key
			if((state.Keys[i/64] >> (i % 64)) & 1) {
                bool wasPressed = (prevState.Keys[i/64] >> (i % 64)) & 1;
                if(!wasPressed){
                    bool isLetter = (i >= 4 && i < 0x1E); //get letter keys to get case
                    codes.push_back((isLetter && !isShift) ? keyMap[i] + 0x20 : keyMap[i]);
                }
            }
		}
        return codes;
    }
	
	Keyboard(const Keyboard&);
	Keyboard& operator=(const Keyboard&);
};
};