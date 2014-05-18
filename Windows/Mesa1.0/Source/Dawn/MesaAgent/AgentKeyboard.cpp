// AgentKeyboard.cpp : implementation of class AgentKeyboard
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentKeyboard.h"
#include "../MesaProcessor/MesaProcessor.h"
#include "../AppFramework/DawnApp.h"
#include "../AppFramework/DawnView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// module AgentKeyboard queue and flushing parameters
typedef enum _QueueMode { kSendKeysDirect = 0, kUseTimerQueue, kUsePollQueue, kUseInstructionQueue } QueueMode;
typedef enum _FlushMode { kFlushOne = 0, kFlushAll } FlushMode;
static QueueMode s_queueMode = kSendKeysDirect;
static FlushMode s_flushMode = kFlushOne;
#define _TIMER_QUEUE_TIMEOUT_IN_MS (30)
#define _INSTRUCTION_QUEUE_TIMEOUT_IN_INSTRUCTIONS (2400)

/////////////////////////////////////////////////////////////////////////////
// module AgentKeyboard private types
typedef enum _XLevelVKeys
{
    kXKey_null              = 0,
    kXKey_Bullet            = 1,
    kXKey_SuperSub          = 2,
    kXKey_Case              = 3,
    kXKey_Strikeout         = 4,
    kXKey_KeypadTwo         = 5,
    kXKey_KeypadThree       = 6,
    kXKey_SingleQuote       = 7,
    kXKey_KeypadAdd         = 8,
    kXKey_KeypadSubtract    = 9,
    kXKey_KeypadMultiply    = 10,
    kXKey_KeypadDivide      = 11,
    kXKey_KeypadClear       = 12,
    kXKey_Point             = 13,
    kXKey_Adjust            = 14,
    kXKey_Menu              = 15,
    kXKey_Five              = 16,
    kXKey_Four              = 17,
    kXKey_Six               = 18,
    kXKey_E                 = 19,
    kXKey_Seven             = 20,
    kXKey_D                 = 21,
    kXKey_U                 = 22,
    kXKey_V                 = 23,
    kXKey_Zero              = 24,
    kXKey_K                 = 25,
    kXKey_Dash              = 26,
    kXKey_P                 = 27,
    kXKey_Slash             = 28,
    kXKey_Font              = 29,
    kXKey_Same              = 30,
    kXKey_BS                = 31,
    kXKey_Three             = 32,
    kXKey_Two               = 33,
    kXKey_W                 = 34,
    kXKey_Q                 = 35,
    kXKey_S                 = 36,
    kXKey_A                 = 37,
    kXKey_Nine              = 38,
    kXKey_I                 = 39,
    kXKey_X                 = 40,
    kXKey_O                 = 41,
    kXKey_L                 = 42,
    kXKey_Comma             = 43,
    kXKey_Quote             = 44,
    kXKey_RightBracket      = 45,
    kXKey_Open              = 46,
    kXKey_Special           = 47,
    kXKey_One               = 48,
    kXKey_Tab               = 49,
    kXKey_ParaTab           = 50,
    kXKey_F                 = 51,
    kXKey_Props             = 52,
    kXKey_C                 = 53,
    kXKey_J                 = 54,
    kXKey_B                 = 55,
    kXKey_Z                 = 56,
    kXKey_LeftShift         = 57,
    kXKey_Period            = 58,
    kXKey_SemiColon         = 59,
    kXKey_NewPara           = 60,
    kXKey_OpenQuote         = 61,
    kXKey_Delete            = 62,
    kXKey_Next              = 63,
    kXKey_R                 = 64,
    kXKey_T                 = 65,
    kXKey_G                 = 66,
    kXKey_Y                 = 67,
    kXKey_H                 = 68,
    kXKey_Eight             = 69,
    kXKey_N                 = 70,
    kXKey_M                 = 71,
    kXKey_Lock              = 72,
    kXKey_Space             = 73,
    kXKey_LeftBracket       = 74,
    kXKey_Equal             = 75,
    kXKey_RightShift        = 76,
    kXKey_Stop              = 77,
    kXKey_Move              = 78,
    kXKey_Undo              = 79,
    kXKey_Margins           = 80,
    kXKey_KeypadSeven       = 81,
    kXKey_KeypadEight       = 82,
    kXKey_KeypadNine        = 83,
    kXKey_KeypadFour        = 84,
    kXKey_KeypadFive        = 85,
    kXKey_English           = 86,
    kXKey_KeypadSix         = 87,
    kXKey_Katakana          = 88,
    kXKey_Copy              = 89,
    kXKey_Find              = 90,
    kXKey_Again             = 91,
    kXKey_Help              = 92,
    kXKey_Expand            = 93,
    kXKey_KeypadOne         = 94,
    kXKey_DiagnosticBitTwo  = 95,
    kXKey_DiagnosticBitOne  = 96,
    kXKey_Center            = 97,
    kXKey_KeypadZero        = 98,
    kXKey_Bold              = 99,
    kXKey_Italic            = 100,
    kXKey_Underline         = 101,
    kXKey_Superscript       = 102,
    kXKey_Subscript         = 103,
    kXKey_Smaller           = 104,
    kXKey_KeypadPeriod      = 105,
    kXKey_KeypadComma       = 106,
    kXKey_LeftShiftAlt      = 107,
    kXKey_DoubleQuote       = 108,
    kXKey_Defaults          = 109,
    kXKey_Hiragana          = 110,
    kXKey_RightShiftAlt     = 111,
} XLevelVKeys;

/////////////////////////////////////////////////////////////////////////////
// module AgentKeyboard private static variables
AgentKeyboard::XKeyMap AgentKeyboard::s_xKeyMap;

AgentKeyboard::XKeyMapEntry AgentKeyboard::s_xKeyMapInitializer[] =
{
    { VK_LBUTTON,   kXKey_Point,            kXKey_Point,            false,  false   },
    { VK_RBUTTON,   kXKey_Adjust,           kXKey_Adjust,           false,  false   },
    { VK_MBUTTON,   kXKey_Menu,             kXKey_Menu,             false,  false   },
    { VK_BACK,      kXKey_null,             kXKey_null,             false,  true    },
    { VK_TAB,       kXKey_null,             kXKey_null,             false,  true    },
    { VK_CLEAR,     kXKey_KeypadFive,       kXKey_KeypadFive,       false,  false   },
    { VK_RETURN,    kXKey_NewPara,          kXKey_KeypadComma,      false,  false   },
    { VK_SHIFT,     kXKey_null,             kXKey_null,             false,  true    },
//  { VK_MENU,      kXKey_LeftShiftAlt,     kXKey_RightShiftAlt,    false,  false   },
    { VK_CAPITAL,   kXKey_Lock,             kXKey_Lock,             true,   false   },
    { VK_FINAL,     kXKey_KeypadSeven,      kXKey_KeypadSeven,      false,  false   },
    { VK_ESCAPE,    kXKey_null,             kXKey_null,             false,  true    },
    { VK_SPACE,     kXKey_null,             kXKey_null,             false,  true    },
    { VK_PRIOR,     kXKey_KeypadNine,       kXKey_Copy,             false,  false   },
    { VK_NEXT,      kXKey_KeypadThree,      kXKey_Move,             false,  false   },
    { VK_END,       kXKey_KeypadOne,        kXKey_Find,             false,  false   },
    { VK_HOME,      kXKey_KeypadSeven,      kXKey_Same,             false,  false   },
    { VK_LEFT,      kXKey_KeypadFour,       kXKey_Expand,           false,  false   },
    { VK_UP,        kXKey_KeypadEight,      kXKey_Open,             false,  false   },
    { VK_RIGHT,     kXKey_KeypadSix,        kXKey_Next,             false,  false   },
    { VK_DOWN,      kXKey_KeypadTwo,        kXKey_Undo,             false,  false   },
    { VK_PRINT,     kXKey_null,             kXKey_null,             false,  true    },
    { VK_INSERT,    kXKey_KeypadZero,       kXKey_Again,            false,  false   },
    { VK_DELETE,    kXKey_KeypadPeriod,     kXKey_Delete,           false,  false   },
    { '0',          kXKey_null,             kXKey_null,             false,  true    },
    { '1',          kXKey_null,             kXKey_null,             false,  true    },
    { '2',          kXKey_null,             kXKey_null,             false,  true    },
    { '3',          kXKey_null,             kXKey_null,             false,  true    },
    { '4',          kXKey_null,             kXKey_null,             false,  true    },
    { '5',          kXKey_null,             kXKey_null,             false,  true    },
    { '6',          kXKey_null,             kXKey_null,             false,  true    },
    { '7',          kXKey_null,             kXKey_null,             false,  true    },
    { '8',          kXKey_null,             kXKey_null,             false,  true    },
    { '9',          kXKey_null,             kXKey_null,             false,  true    },
    { 'A',          kXKey_null,             kXKey_null,             false,  true    },
    { 'B',          kXKey_null,             kXKey_null,             false,  true    },
    { 'C',          kXKey_null,             kXKey_null,             false,  true    },
    { 'D',          kXKey_null,             kXKey_null,             false,  true    },
    { 'E',          kXKey_null,             kXKey_null,             false,  true    },
    { 'F',          kXKey_null,             kXKey_null,             false,  true    },
    { 'G',          kXKey_null,             kXKey_null,             false,  true    },
    { 'H',          kXKey_null,             kXKey_null,             false,  true    },
    { 'I',          kXKey_null,             kXKey_null,             false,  true    },
    { 'J',          kXKey_null,             kXKey_null,             false,  true    },
    { 'K',          kXKey_null,             kXKey_null,             false,  true    },
    { 'L',          kXKey_null,             kXKey_null,             false,  true    },
    { 'M',          kXKey_null,             kXKey_null,             false,  true    },
    { 'N',          kXKey_null,             kXKey_null,             false,  true    },
    { 'O',          kXKey_null,             kXKey_null,             false,  true    },
    { 'P',          kXKey_null,             kXKey_null,             false,  true    },
    { 'Q',          kXKey_null,             kXKey_null,             false,  true    },
    { 'R',          kXKey_null,             kXKey_null,             false,  true    },
    { 'S',          kXKey_null,             kXKey_null,             false,  true    },
    { 'T',          kXKey_null,             kXKey_null,             false,  true    },
    { 'U',          kXKey_null,             kXKey_null,             false,  true    },
    { 'V',          kXKey_null,             kXKey_null,             false,  true    },
    { 'W',          kXKey_null,             kXKey_null,             false,  true    },
    { 'X',          kXKey_null,             kXKey_null,             false,  true    },
    { 'Y',          kXKey_null,             kXKey_null,             false,  true    },
    { 'Z',          kXKey_null,             kXKey_null,             false,  true    },
    { VK_NUMPAD0,   kXKey_KeypadZero,       kXKey_KeypadZero,       false,  false   },
    { VK_NUMPAD1,   kXKey_KeypadOne,        kXKey_KeypadOne,        false,  false   },
    { VK_NUMPAD2,   kXKey_KeypadTwo,        kXKey_KeypadTwo,        false,  false   },
    { VK_NUMPAD3,   kXKey_KeypadThree,      kXKey_KeypadThree,      false,  false   },
    { VK_NUMPAD4,   kXKey_KeypadFour,       kXKey_KeypadFour,       false,  false   },
    { VK_NUMPAD5,   kXKey_KeypadFive,       kXKey_KeypadFive,       false,  false   },
    { VK_NUMPAD6,   kXKey_KeypadSix,        kXKey_KeypadSix,        false,  false   },
    { VK_NUMPAD7,   kXKey_KeypadSeven,      kXKey_KeypadSeven,      false,  false   },
    { VK_NUMPAD8,   kXKey_KeypadEight,      kXKey_KeypadEight,      false,  false   },
    { VK_NUMPAD9,   kXKey_KeypadNine,       kXKey_KeypadNine,       false,  false   },
    { VK_MULTIPLY,  kXKey_KeypadMultiply,   kXKey_KeypadMultiply,   false,  false   },
    { VK_ADD,       kXKey_KeypadAdd,        kXKey_KeypadAdd,        false,  false   },
    { VK_SUBTRACT,  kXKey_KeypadSubtract,   kXKey_KeypadSubtract,   false,  false   },
    { VK_DECIMAL,   kXKey_KeypadPeriod,     kXKey_KeypadPeriod,     false,  false   },
    { VK_DIVIDE,    kXKey_KeypadDivide,     kXKey_KeypadDivide,     false,  false   },
    { VK_F1,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F2,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F3,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F4,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F5,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F6,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F7,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F8,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F9,        kXKey_null,             kXKey_null,             false,  true    },
    { VK_F10,       kXKey_null,             kXKey_null,             false,  true    },
    { VK_F11,       kXKey_null,             kXKey_null,             false,  true    },
    { VK_F12,       kXKey_null,             kXKey_null,             false,  true    },
    { VK_NUMLOCK,   kXKey_KeypadClear,      kXKey_KeypadClear,      true,   false   },
    { VK_SCROLL,    kXKey_Help,             kXKey_Help,             true,   false   },
    { 0xBA,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xBB,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xBC,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xBD,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xBE,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xBF,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xC0,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xDB,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xDC,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xDD,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xDE,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xDF,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xE0,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xE1,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xE2,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xE3,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xE4,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xE6,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xE9,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xEA,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xEB,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xEC,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xED,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xEE,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xEF,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xF0,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xF1,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xF2,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xF3,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xF4,         kXKey_null,             kXKey_null,             false,  true    },
    { 0xF5,         kXKey_null,             kXKey_null,             false,  true    },
};

AgentKeyboard::XKeyMapEntry AgentKeyboard::s_xKeyMapInitializerOEM[] =
{
    { 1,            kXKey_Stop,             kXKey_Stop,             false,  false   },
    { 2,            kXKey_One,              kXKey_One,              false,  false   },
    { 3,            kXKey_Two,              kXKey_Two,              false,  false   },
    { 4,            kXKey_Three,            kXKey_Three,            false,  false   },
    { 5,            kXKey_Four,             kXKey_Four,             false,  false   },
    { 6,            kXKey_Five,             kXKey_Five,             false,  false   },
    { 7,            kXKey_Six,              kXKey_Six,              false,  false   },
    { 8,            kXKey_Seven,            kXKey_Seven,            false,  false   },
    { 9,            kXKey_Eight,            kXKey_Eight,            false,  false   },
    { 10,           kXKey_Nine,             kXKey_Nine,             false,  false   },
    { 11,           kXKey_Zero,             kXKey_Zero,             false,  false   },
    { 12,           kXKey_Dash,             kXKey_Dash,             false,  false   },
    { 13,           kXKey_Equal,            kXKey_Equal,            false,  false   },
    { 14,           kXKey_BS,               kXKey_BS,               false,  false   },
    { 15,           kXKey_ParaTab,          kXKey_ParaTab,          false,  false   },
    { 16,           kXKey_Q,                kXKey_Q,                false,  false   },
    { 17,           kXKey_W,                kXKey_W,                false,  false   },
    { 18,           kXKey_E,                kXKey_E,                false,  false   },
    { 19,           kXKey_R,                kXKey_R,                false,  false   },
    { 20,           kXKey_T,                kXKey_T,                false,  false   },
    { 21,           kXKey_Y,                kXKey_Y,                false,  false   },
    { 22,           kXKey_U,                kXKey_U,                false,  false   },
    { 23,           kXKey_I,                kXKey_I,                false,  false   },
    { 24,           kXKey_O,                kXKey_O,                false,  false   },
    { 25,           kXKey_P,                kXKey_P,                false,  false   },
    { 26,           kXKey_LeftBracket,      kXKey_LeftBracket,      false,  false   },
    { 27,           kXKey_RightBracket,     kXKey_RightBracket,     false,  false   },
    { 28,           kXKey_NewPara,          kXKey_NewPara,          false,  false   },
//  { 28,           kXKey_KeypadAdd,        kXKey_KeypadAdd,        false,  false   },
    { 30,           kXKey_A,                kXKey_A,                false,  false   },
    { 31,           kXKey_S,                kXKey_S,                false,  false   },
    { 32,           kXKey_D,                kXKey_D,                false,  false   },
    { 33,           kXKey_F,                kXKey_F,                false,  false   },
    { 34,           kXKey_G,                kXKey_G,                false,  false   },
    { 35,           kXKey_H,                kXKey_H,                false,  false   },
    { 36,           kXKey_J,                kXKey_J,                false,  false   },
    { 37,           kXKey_K,                kXKey_K,                false,  false   },
    { 38,           kXKey_L,                kXKey_L,                false,  false   },
    { 39,           kXKey_SemiColon,        kXKey_SemiColon,        false,  false   },
    { 40,           kXKey_SingleQuote,      kXKey_SingleQuote,      false,  false   },
    { 41,           kXKey_Bullet,           kXKey_Bullet,           false,  false   },
    { 42,           kXKey_LeftShift,        kXKey_LeftShift,        false,  false   },
    { 43,           kXKey_DoubleQuote,      kXKey_DoubleQuote,      false,  false   },
    { 44,           kXKey_Z,                kXKey_Z,                false,  false   },
    { 45,           kXKey_X,                kXKey_X,                false,  false   },
    { 46,           kXKey_C,                kXKey_C,                false,  false   },
    { 47,           kXKey_V,                kXKey_V,                false,  false   },
    { 48,           kXKey_B,                kXKey_B,                false,  false   },
    { 49,           kXKey_N,                kXKey_N,                false,  false   },
    { 50,           kXKey_M,                kXKey_M,                false,  false   },
    { 51,           kXKey_Comma,            kXKey_Comma,            false,  false   },
    { 52,           kXKey_Period,           kXKey_Period,           false,  false   },
    { 53,           kXKey_Slash,            kXKey_Slash,            false,  false   },
//  { 53,           kXKey_KeypadDivide,     kXKey_KeypadDivide,     false,  false   },
    { 54,           kXKey_RightShift,       kXKey_RightShift,       false,  false   },
    { 55,           kXKey_KeypadMultiply,   kXKey_KeypadMultiply,   false,  false   },
    { 56,           kXKey_LeftShiftAlt,     kXKey_LeftShiftAlt,     false,  false   },
    { 57,           kXKey_Space,            kXKey_Space,            false,  false   },
    { 59,           kXKey_Center,           kXKey_Center,           false,  false   },
    { 60,           kXKey_Bold,             kXKey_Bold,             false,  false   },
    { 61,           kXKey_Italic,           kXKey_Italic,           false,  false   },
    { 62,           kXKey_Case,             kXKey_Case,             false,  false   },
    { 63,           kXKey_Strikeout,        kXKey_Strikeout,        false,  false   },
    { 64,           kXKey_Underline,        kXKey_Underline,        false,  false   },
    { 65,           kXKey_SuperSub,         kXKey_SuperSub,         false,  false   },
    { 66,           kXKey_Smaller,          kXKey_Smaller,          false,  false   },
    { 67,           kXKey_Margins,          kXKey_Margins,          false,  false   },
    { 68,           kXKey_Font,             kXKey_Font,             false,  false   },
    { 74,           kXKey_KeypadSubtract,   kXKey_KeypadSubtract,   false,  false   },
    { 76,           kXKey_KeypadFive,       kXKey_KeypadFive,       false,  false   },
    { 78,           kXKey_KeypadAdd,        kXKey_KeypadAdd,        false,  false   },
    { 86,           kXKey_LeftShiftAlt,     kXKey_LeftShiftAlt,     false,  false   },
    { 87,           kXKey_Special,          kXKey_Special,          false,  false   },
    { 88,           kXKey_Props,            kXKey_Props,            false,  false   },
};


/////////////////////////////////////////////////////////////////////////////
// class XKeyMap
void
AgentKeyboard::XKeyMap::InitializeMap()
{
    u32 i;
    u32 nSize = sizeof(s_xKeyMapInitializer) / sizeof(XKeyMapEntry);
    
    memset(m_map, 0, sizeof(m_map));
    memset(m_oemMap, 0, sizeof(m_oemMap));
    
    for (i = 0; i < nSize; i++) m_map[s_xKeyMapInitializer[i].m_nVK] = s_xKeyMapInitializer[i];

    nSize = sizeof(s_xKeyMapInitializerOEM) / sizeof(XKeyMapEntry);
    for (i = 0; i < nSize; i++) m_oemMap[s_xKeyMapInitializerOEM[i].m_nVK] = s_xKeyMapInitializerOEM[i];
}

AgentKeyboard::XKeyStroke
AgentKeyboard::XKeyMap::Lookup(u32 nVKKey, u32 nFlags)
{
    AgentKeyboard::XKeyStroke keystroke;
    keystroke.SetKeyCode(kXKey_null);

    if (nVKKey > kMaxVK) keystroke;

    XKeyMapEntry entry = m_map[nVKKey];
    if (entry.m_bUseOEM)
    {
        nVKKey = nFlags & 0xFF;
        entry = m_oemMap[nVKKey];
    }

    keystroke.SetKeyCode((nFlags & (1 << 8)) ? entry.m_xKeyExtended : entry.m_xKeyStandard);
    keystroke.SetKeyToggle(entry.m_bToggleKey);

    return keystroke;
}

/////////////////////////////////////////////////////////////////////////////
// module AgentKeyboard private static functions
static void CALLBACK EXPORT
StaticTimerProc(HWND hWnd, UINT nMsgID, UINT nTimerID, DWORD dwTime)
{
    ((AgentKeyboard *)nTimerID)->OnTimer();
}

/////////////////////////////////////////////////////////////////////////////
// class AgentKeyboard
AgentKeyboard::AgentKeyboard(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pKeyboardFCB = (FCB *)pFCB;
    DawnView * pView = DawnApp::GetDawnView();

    memset(pKeyboardFCB, -1, sizeof(FCB));
    s_xKeyMap.InitializeMap();
    m_queue.SetKeyboardAgent(this);

    m_pTimer = (s_queueMode == kUseTimerQueue) ? ((0 == pView->SetTimer((UINT)this, _TIMER_QUEUE_TIMEOUT_IN_MS, &StaticTimerProc)) ? NULL : (void *)pView) : NULL;
}

AgentKeyboard::~AgentKeyboard()
{
    if (m_pTimer)
    {
        ((DawnView *)m_pTimer)->KillTimer((UINT)this);
    }
}

/////////////////////////////////////////////////////////////////////////////
// class AgentKeyboard public virtual functions
bool
AgentKeyboard::Initiate()
{
    return true;
}

bool
AgentKeyboard::Poll()
{
    switch (s_queueMode)
    {
        case kUsePollQueue:
            m_queue.Flush();
            break;
        case kUseInstructionQueue:
            m_queue.FlushInstructionQueue();
            break;
        default:
            break;
    }
    return false;
}

u32
AgentKeyboard::GetFCBSize()
{
    return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentKeyboard public functions
void
AgentKeyboard::SetMouseButtonState(int nVKButton, bool bDown, bool bOtherMouseButtonsDown)
{
    DawnView * pView = DawnApp::GetDawnView();
    if (bDown)
    {
        if (pView->GetCapture() != pView) pView->SetCapture();
    }
    else
    {
        if ((false == bOtherMouseButtonsDown) && (pView->GetCapture() == pView)) ::ReleaseCapture();
    }

    SetKeyState(nVKButton, 0, bDown);
}

void
AgentKeyboard::SetKeyState(u32 nVKKey, u32 nFlags, bool bDown)
{
    XKeyStroke keystroke = s_xKeyMap.Lookup(nVKKey, nFlags);
    if (kXKey_null != keystroke.GetKeyCode() && ((! keystroke.IsKeyToggle()) || bDown))
    {
        if (keystroke.IsKeyToggle()) bDown = (::GetKeyState(nVKKey) & 1) ? true : false;
        keystroke.SetKeyDown(bDown);
        if (s_queueMode == kSendKeysDirect) SetXeroxKeyStroke(keystroke);
        else m_queue.Add(keystroke);
        if (kUseInstructionQueue == s_queueMode) m_queue.FlushInstructionQueue(); 
    }
}

void
AgentKeyboard::SetToggleKeyState()
{
    // set the state of all of the toggle keys (caps lock, num lock, etc)
    u32 nSize = sizeof(s_xKeyMapInitializer) / sizeof(XKeyMapEntry);
    for (u32 i = 0; i < nSize; i++)
    {
        if (s_xKeyMapInitializer[i].m_bToggleKey) SetKeyState(s_xKeyMapInitializer[i].m_nVK, 0, true);
    }
}

/////////////////////////////////////////////////////////////////////////////
// class AgentKeyboard private functions
void
AgentKeyboard::SetXeroxKeyStroke(XKeyStroke keystroke)
{
    u8 nXeroxKeyCode = keystroke.GetKeyCode();

#if 0
    if (keystroke.IsKeyToggle())
    {
        bool bUp = (((FCB *)m_pFCB)->m_keyState[nXeroxKeyCode / 16] & (1 << (15 - (nXeroxKeyCode % 16)))) ? true : false;
        keystroke.SetKeyDown(bUp);
    }
#endif

    if (keystroke.IsKeyDown())
    {
        ((FCB *)m_pFCB)->m_keyState[nXeroxKeyCode / 16] &= ~ (1 << (15 - (nXeroxKeyCode % 16)));
    }
    else
    {
        ((FCB *)m_pFCB)->m_keyState[nXeroxKeyCode / 16] |=   (1 << (15 - (nXeroxKeyCode % 16)));
    }
}

void
AgentKeyboard::OnTimer()
{
    m_queue.Flush();
}

/////////////////////////////////////////////////////////////////////////////
// class AgentKeyboard::KeystrokeQueue private functions
void
AgentKeyboard::KeystrokeQueue::Add(AgentKeyboard::XKeyStroke keystroke)
{
    if (m_nNumKeystrokes == kQueueSize)
    {
        TRACE("KeystrokeQueue FULL!\n");
        return;
    }
    m_nNumKeystrokes++;
    if ((++m_nLastKeystroke) >= kQueueSize) m_nLastKeystroke = 0;
    m_keystrokes[m_nLastKeystroke] = keystroke;
}

void
AgentKeyboard::KeystrokeQueue::Flush()
{
    if (0 == m_nNumKeystrokes) return;
    if (kFlushOne == s_flushMode)
    {
        m_pAgent->SetXeroxKeyStroke(m_keystrokes[m_nFirstKeystroke]);
        m_nNumKeystrokes--;
        if (0 == m_nNumKeystrokes) Clear();
        else if ((++m_nFirstKeystroke) >= kQueueSize) m_nFirstKeystroke = 0;
    }
    else
    {
        u32 nKeystroke = m_nFirstKeystroke;
        while (! SawKeystroke(nKeystroke))
        {
            m_pAgent->SetXeroxKeyStroke(m_keystrokes[nKeystroke]);
            m_nNumKeystrokes--;
            if (nKeystroke == m_nLastKeystroke) break;
            if ((++nKeystroke) >= kQueueSize) nKeystroke = 0;
        }
        if (0 == m_nNumKeystrokes) Clear();
        else m_nFirstKeystroke = nKeystroke;
    }
}

bool
AgentKeyboard::KeystrokeQueue::SawKeystroke(u32 nKeystroke)
{
    u8 keycode = m_keystrokes[nKeystroke].GetKeyCode();
    u32 nKeystrokeToCheck = m_nFirstKeystroke;
    while (nKeystrokeToCheck != nKeystroke)
    {
        if (m_keystrokes[nKeystrokeToCheck].GetKeyCode() == keycode) return true;
        if ((++nKeystrokeToCheck) >= kQueueSize) nKeystrokeToCheck = 0;
    }
    return false;
}

void
AgentKeyboard::KeystrokeQueue::FlushInstructionQueue()
{
    static u32 s_nInstructionsCountOnLastFlush = 0;
    if (0 == m_nNumKeystrokes) return;

    u32 nInstructionCount = MesaProcessor::GetNumExecutedInstructions();
    if (nInstructionCount > s_nInstructionsCountOnLastFlush) 
    {
        if ((nInstructionCount - s_nInstructionsCountOnLastFlush) > _INSTRUCTION_QUEUE_TIMEOUT_IN_INSTRUCTIONS)
        {
            Flush();
            s_nInstructionsCountOnLastFlush = nInstructionCount;
        }
    }
    else if (nInstructionCount < s_nInstructionsCountOnLastFlush)
    {
        if ((nInstructionCount + (0xFFFFFFFF - s_nInstructionsCountOnLastFlush)) > _INSTRUCTION_QUEUE_TIMEOUT_IN_INSTRUCTIONS) 
        {
            Flush();
            s_nInstructionsCountOnLastFlush = nInstructionCount;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
