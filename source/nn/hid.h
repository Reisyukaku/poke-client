/**
 * @file hid.h
 * @brief Functions that help process gamepad inputs.
 */

#pragma once

#include "types.h"

namespace nn
{
    namespace hid
    {
        struct MouseState {
            s64 sampleNum;
            s32 x;
            s32 y;
            s32 dx;
            s32 dy;
            s32 wheelDelta;
            s32 reserved;
            u32 buttons;
            u32 attrib;
        };
		
		struct KeyboardState {
            s64 sampleNum;
            u32 Modifier;
            u32 Attribs;
            u64 Keys[4];
        };
        
        enum KeyboardModifier {
            
        };
        
        enum KeyboardAttribute {
            IsConnected = 0
        };
        
        enum KeyboardKey {
            A = 0x04,
            B = 0x05,
            C = 0x06,
            D = 0x07,
            E = 0x08,
            F = 0x09,
            G = 0x0A,
            H = 0x0B,
            I = 0x0C,
            J = 0x0D,
            K = 0x0E,
            L = 0x0F,
            M = 0x10,
            N = 0x11,
            O = 0x12,
            P = 0x13,
            Q = 0x14,
            R = 0x15,
            S = 0x16,
            T = 0x17,
            U = 0x18,
            V = 0x19,
            W = 0x1A,
            X = 0x1B,
            Y = 0x1C,
            Z = 0x1D,
            D1 = 0x1E,
            D2 = 0x1F,
            D3 = 0x20,
            D4 = 0x21,
            D5 = 0x22,
            D6 = 0x23,
            D7 = 0x24,
            D8 = 0x25,
            D9 = 0x26,
            D0 = 0x27,
            Return = 0x28,
            Escape = 0x29,
            Backspace = 0x2A,
            Tab = 0x2B,
            Space = 0x2C,
            Minus = 0x2D,
            Plus = 0x2E,
            OpenBracket = 0x2F,
            CloseBracket = 0x30,
            Pipe = 0x31,
            Tilde = 0x32,
            Semicolon = 0x33,
            Quote = 0x34,
            Backquote = 0x35,
            Comma = 0x36,
            Period = 0x37,
            Slash = 0x38,
            CapsLock = 0x39,
            F1 = 0x3A,
            F2 = 0x3B,
            F3 = 0x3C,
            F4 = 0x3D,
            F5 = 0x3E,
            F6 = 0x3F,
            F7 = 0x40,
            F8 = 0x41,
            F9 = 0x42,
            F10 = 0x43,
            F11 = 0x44,
            F12 = 0x45,
            PrintScreen = 0x46,
            ScrollLock = 0x47,
            Pause = 0x48,
            Insert = 0x49,
            Home = 0x4A,
            PageUp = 0x4B,
            Delete = 0x4C,
            End = 0x4D,
            PageDown = 0x4E,
            RightArrow = 0x4F,
            LeftArrow = 0x50,
            DownArrow = 0x51,
            UpArrow = 0x52,
            NumLock = 0x53,
            NumPadDivide = 0x54,
            NumPadMultiply = 0x55,
            NumPadSubtract = 0x56,
            NumPadAdd = 0x57,
            NumPadEnter = 0x58,
            NumPad1 = 0x59,
            NumPad2 = 0x5A,
            NumPad3 = 0x5B,
            NumPad4 = 0x5C,
            NumPad5 = 0x5D,
            NumPad6 = 0x5E,
            NumPad7 = 0x5F,
            NumPad8 = 0x60,
            NumPad9 = 0x61,
            NumPad0 = 0x62,
            NumPadDot = 0x63,
            Backslash = 0x64,
            Application = 0x65,
            Power = 0x66,
            NumPadEquals = 0x67,
            F13 = 0x68,
            F14 = 0x69,
            F15 = 0x6A,
            F16 = 0x6B,
            F17 = 0x6C,
            F18 = 0x6D,
            F19 = 0x6E,
            F20 = 0x6F,
            F21 = 0x70,
            F22 = 0x71,
            F23 = 0x72,
            F24 = 0x73,
            NumPadComma = 0x85,
            Ro = 0x87,
            KatakanaHiragana = 0x88,
            Yen = 0x89,
            Henkan = 0x8A,
            Muhenkan = 0x8B,
            NumPadCommaPc98 = 0x8C,
            HangulEnglish = 0x90,
            Hanja = 0x91,
            Katakana = 0x92,
            Hiragana = 0x93,
            ZenkakuHankaku = 0x94,
            LeftControl = 0xE0,
            LeftShift = 0xE1,
            LeftAlt = 0xE2,
            LeftGui = 0xE3,
            RightControl = 0xE4,
            RightShift = 0xE5,
            RightAlt = 0xE6,
            RightGui = 0xE7
        };
        
        /*nn::hid::KeyboardLayout::Portuguese
        nn::hid::KeyboardLayout::KoreanLatin
        nn::hid::KeyboardLayout::FrenchCanada
        nn::hid::KeyboardLayout::KoreanHangul
        nn::hid::KeyboardLayout::RussianLatin
        nn::hid::KeyboardLayout::RussianCyrillic
        nn::hid::KeyboardLayout::ChineseSimplified
        nn::hid::KeyboardLayout::EnglishUnitedStates
        nn::hid::KeyboardLayout::SpanishLatinAmerica
        nn::hid::KeyboardLayout::EnglishUnitedKingdom
        nn::hid::KeyboardLayout::ChineseTraditionalLatin
        nn::hid::KeyboardLayout::ChineseTraditionalZhuyin
        nn::hid::KeyboardLayout::ChineseTraditionalCangjie
        nn::hid::KeyboardLayout::EnglishUnitedStatesInternational
        nn::hid::KeyboardLayout::French
        nn::hid::KeyboardLayout::German
        nn::hid::KeyboardLayout::Italian
        nn::hid::KeyboardLayout::Spanish
        nn::hid::KeyboardLayout::Japanese*/
        
        enum KeyboardLayout {
            Portuguese,
            KoreanLatin,
            FrenchCanada,
            KoreanHangul,
            RussianLatin,
            RussianCyrillic,
            ChineseSimplified,
            EnglishUnitedStates,
            SpanishLatinAmerica,
            EnglishUnitedKingdom,
            ChineseTraditionalLatin,
            ChineseTraditionalZhuyin,
            ChineseTraditionalCangjie,
            EnglishUnitedStatesInternational,
            French,
            German,
            Italian,
            Spanish,
            Japanese
        };

		enum MouseButtons {
			LeftClick = 1 << 0,
			RightClick = 1 << 1,
			MiddleClick = 1 << 2,
			Extra1 = 1 << 3,
			Extra2 = 1 << 4
		};
        
        void InitializeMouse();
        void InitializeKeyboard();
        void GetMouseState(nn::hid::MouseState*);
        void GetKeyboardState(nn::hid::KeyboardState*);
        void GetKeyCode(unsigned short*, int, u32, nn::hid::KeyboardLayout);
    };
};