#include "VarDefine.h"



struct SysTime_REG SysTime = {0};



//键位表
const uint8_t KeyValueDef[KEY_COL_NUM][KEY_ROW_NUM] = {
{ Keypad_ADD,         Keypad_SUB,             Keypad_NumLock,         Keypad_DIV, 
  0,                  Keypad_ENTER,           Keyboard_LeftControl,   Keyboard_LeftShift, 
  Keyboard_CapsLock,  Keyboard_Tab,           Keyboard_OEM3,          Keyboard_ESCAPE,      },
{ Keypad_6,           Keypad_MULT,            Keyboard_PageUp,        Keyboard_PageDown,
  Keypad_3,           Keypad_DEC,             Keyboard_LeftGUI,       Keyboard_z, 
  Keyboard_a,         Keyboard_q,             Keyboard_1,             Keyboard_F1,          },
{ Keypad_9,           Keyboard_Pause,         Keyboard_Home,          Keyboard_End, 
  Keypad_2,           Keypad_0,               Keyboard_LeftAlt,       Keyboard_x, 
  Keyboard_s,         Keyboard_w,             Keyboard_2,             Keyboard_F2,          },
{ Keypad_8,           Keyboard_PrintScreen,   Keyboard_Insert,        Keyboard_Delete,  
  Keypad_1,           Keyboard_RightArrow,    Keyboard_Space,         Keyboard_c, 
  Keyboard_d,         Keyboard_e,             Keyboard_3,             Keyboard_F3,          },
{ Keypad_5,           Keyboard_ScrollLock,    Keyboard_Backspace,     Keyboard_OEM5,  
  Keyboard_UpArrow,   Keyboard_DownArrow,     0,                      Keyboard_v,
  Keyboard_f,         Keyboard_r,             Keyboard_4,             Keyboard_F4,          },
{ Keypad_4,           Keyboard_F12,           Keyboard_PLUS,          Keyboard_RightBrackets, 
  Keyboard_ENTER,     Keyboard_LeftArrow,     Keyboard_RightControl,  Keyboard_b, 
  Keyboard_g,         Keyboard_t,             Keyboard_5,             Keyboard_F5,          },
{ Keypad_7,           Keyboard_F11,           Keyboard_MINUS,         Keyboard_LeftBrackets,  
  Keyboard_OEM7,      Keyboard_RightShift,    Keyboard_Apps,          Keyboard_n, 
  Keyboard_h,         Keyboard_y,             Keyboard_6,             Keyboard_F6,          },
{ 0,                  Keyboard_F10,           Keyboard_0,             Keyboard_p, 
  Keyboard_OEM1,      Keyboard_DIV,           Keyboard_RightGUI,      Keyboard_m, 
  Keyboard_j,         Keyboard_u,             Keyboard_7,             Keyboard_F7,          },
{ 0,                  Keyboard_F9,            Keyboard_9,             Keyboard_o, 
  Keyboard_l,         Keyboard_PERIOD,        Keyboard_RightAlt,      Keyboard_COMMA, 
  Keyboard_k,         Keyboard_i,             Keyboard_8,             Keyboard_F8           },
};

//键位表
const uint8_t KeyFNValueDef[KEY_COL_NUM][KEY_ROW_NUM] = {
{ Keypad_ADD,         Keypad_SUB,             Keypad_NumLock,         Keypad_DIV, 
  0,                  Keypad_ENTER,           Keyboard_LeftControl,   Keyboard_LeftShift, 
  Keyboard_CapsLock,  Keyboard_Tab,           Keyboard_OEM3,          Keyboard_ESCAPE,      },
{ Keypad_6,           Keypad_MULT,            Keyboard_PageUp,        Keyboard_PageDown,
  Keypad_3,           Keypad_DEC,             Keyboard_LeftGUI,       Keyboard_z, 
  Keyboard_a,         Keyboard_q,             Keyboard_1,             Keyboard_F1,          },
{ Keypad_9,           Keyboard_Pause,         Keyboard_Home,          Keyboard_End, 
  Keypad_2,           Keypad_0,               Keyboard_LeftAlt,       Keyboard_x, 
  Keyboard_s,         Keyboard_w,             Keyboard_2,             Keyboard_F2,          },
{ Keypad_8,           Keyboard_PrintScreen,   Keyboard_Insert,        Keyboard_Delete,  
  Keypad_1,           Keyboard_RightArrow,    Keyboard_Space,         Keyboard_c, 
  Keyboard_d,         Keyboard_e,             Keyboard_3,             Keyboard_F3,          },
{ Keypad_5,           Keyboard_ScrollLock,    Keyboard_Backspace,     Keyboard_OEM5,  
  Keyboard_UpArrow,   Keyboard_DownArrow,     0,                      Keyboard_v,
  Keyboard_f,         Keyboard_r,             Keyboard_4,             Keyboard_F4,          },
{ Keypad_4,           Keyboard_F12,           Keyboard_PLUS,          Keyboard_RightBrackets, 
  Keyboard_ENTER,     Keyboard_LeftArrow,     Keyboard_RightControl,  Keyboard_b, 
  Keyboard_g,         Keyboard_t,             Keyboard_5,             Keyboard_F5,          },
{ Keypad_7,           Keyboard_F11,           Keyboard_MINUS,         Keyboard_LeftBrackets,  
  Keyboard_OEM7,      Keyboard_RightShift,    Keyboard_Apps,          Keyboard_n, 
  Keyboard_h,         Keyboard_y,             Keyboard_6,             Keyboard_F6,          },
{ 0,                  Keyboard_F10,           Keyboard_0,             Keyboard_p, 
  Keyboard_OEM1,      Keyboard_DIV,           Keyboard_RightGUI,      Keyboard_m, 
  Keyboard_j,         Keyboard_u,             Keyboard_7,             Keyboard_F7,          },
{ 0,                  Keyboard_F9,            Keyboard_9,             Keyboard_o, 
  Keyboard_l,         Keyboard_PERIOD,        Keyboard_RightAlt,      Keyboard_COMMA, 
  Keyboard_k,         Keyboard_i,             Keyboard_8,             Keyboard_F8           },
};


//按键键值
uint8_t key_fresh = 0;
uint8_t key_buff[KEY_COL_NUM * KEY_ROW_NUM] = {0};
uint8_t key_buff2 = 0;  //GUI/SHIFT键等
uint8_t key_buff3 = 0;  //media键

//hid usb 发送
uint8_t KeyboardOutData[HID_EPOUT_SIZE];
uint8_t KeyboardInData[HID_EPIN_SIZE] = {0};

//同时保持的按键数量
uint8_t key_keep_num = 6;



uint8_t SysState = 0;
















