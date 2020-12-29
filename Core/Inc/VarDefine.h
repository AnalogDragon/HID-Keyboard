#ifndef _VarDefine_
#define _VarDefine_
#include "main.h"


struct SysTime_REG
{
	volatile uint16_t SysTimeCNT1ms;
	volatile uint16_t SysTimeCNT10ms;
	volatile uint16_t SysTimeCNT100ms;
	volatile uint16_t SysTimeCNT1s;
	volatile uint16_t SysTimeCNT1min;
	volatile uint16_t SysTimeCNT1h;
		
	volatile uint8_t SysTimeFLG1ms;
	volatile uint8_t SysTimeFLG10ms;
	volatile uint8_t SysTimeFLG100ms;
	volatile uint8_t SysTimeFLG1s;
	volatile uint8_t SysTimeFLG1min;
	volatile uint8_t SysTimeFLG1h;
};  


/*----------------------------*/

#define KEY_COL_NUM           9
#define KEY_ROW_NUM           12


#define Keyboard_a                4   // Keyboard a and A
#define Keyboard_b                5   // Keyboard b and B
#define Keyboard_c                6   // Keyboard c and C
#define Keyboard_d                7   // Keyboard d and D
#define Keyboard_e                8   // Keyboard e and E
#define Keyboard_f                9   // Keyboard f and F
#define Keyboard_g                10  // Keyboard g and G
#define Keyboard_h                11  // Keyboard h and H
#define Keyboard_i                12  // Keyboard i and I
#define Keyboard_j                13  // Keyboard j and J
#define Keyboard_k                14  // Keyboard k and K
#define Keyboard_l                15  // Keyboard l and L
#define Keyboard_m                16  // Keyboard m and M
#define Keyboard_n                17  // Keyboard n and N
#define Keyboard_o                18  // Keyboard o and O
#define Keyboard_p                19  // Keyboard p and P
#define Keyboard_q                20  // Keyboard q and Q
#define Keyboard_r                21  // Keyboard r and R
#define Keyboard_s                22  // Keyboard s and S
#define Keyboard_t                23  // Keyboard t and T
#define Keyboard_u                24  // Keyboard u and U
#define Keyboard_v                25  // Keyboard v and V
#define Keyboard_w                26  // Keyboard w and W
#define Keyboard_x                27  // Keyboard x and X
#define Keyboard_y                28  // Keyboard y and Y
#define Keyboard_z                29  // Keyboard z and Z

#define Keyboard_OEM3             53  // Keyboard ` and ~
#define Keyboard_1                30  // Keyboard 1 and !
#define Keyboard_2                31  // Keyboard 2 and @
#define Keyboard_3                32  // Keyboard 3 and #
#define Keyboard_4                33  // Keyboard 4 and $
#define Keyboard_5                34  // Keyboard 5 and %
#define Keyboard_6                35  // Keyboard 6 and ^
#define Keyboard_7                36  // Keyboard 7 and &
#define Keyboard_8                37  // Keyboard 8 and *
#define Keyboard_9                38  // Keyboard 9 and (
#define Keyboard_0                39  // Keyboard 0 and )

#define Keyboard_ENTER            40  // Keyboard ENTER
#define Keyboard_ESCAPE           41  // Keyboard ESCAPE
#define Keyboard_Backspace        42  // Keyboard Backspace
#define Keyboard_Tab              43  // Keyboard Tab
#define Keyboard_Space            44  // Keyboard Spacebar
#define Keyboard_CapsLock         57  // Keyboard Caps Lock

#define Keyboard_MINUS            45  // Keyboard - and _
#define Keyboard_PLUS             46  // Keyboard = and +
#define Keyboard_LeftBrackets     47  // Keyboard [ and {
#define Keyboard_RightBrackets    48  // Keyboard ] and }
#define Keyboard_OEM5             49  // Keyboard \ and |
#define Keyboard_OEM1             51  // Keyboard ; and :
#define Keyboard_OEM7             52  // Keyboard ‘ and “
#define Keyboard_COMMA            54  // Keyboard, and <
#define Keyboard_PERIOD           55  // Keyboard . and >
#define Keyboard_DIV              56  // Keyboard / and ?

#define Keyboard_F1               58  // Keyboard F1
#define Keyboard_F2               59  // Keyboard F2
#define Keyboard_F3               60  // Keyboard F3
#define Keyboard_F4               61  // Keyboard F4
#define Keyboard_F5               62  // Keyboard F5
#define Keyboard_F6               63  // Keyboard F6
#define Keyboard_F7               64  // Keyboard F7
#define Keyboard_F8               65  // Keyboard F8
#define Keyboard_F9               66  // Keyboard F9
#define Keyboard_F10              67  // Keyboard F10
#define Keyboard_F11              68  // Keyboard F11
#define Keyboard_F12              69  // Keyboard F12

#define Keyboard_PrintScreen      70  // Keyboard PrintScreen
#define Keyboard_ScrollLock       71  // Keyboard Scroll Lock
#define Keyboard_Pause            72  // Keyboard Pause

#define Keyboard_Insert           73  // Keyboard Insert
#define Keyboard_Home             74  // Keyboard Home
#define Keyboard_PageUp           75  // Keyboard PageUp
#define Keyboard_Delete           76  // Keyboard Delete
#define Keyboard_End              77  // Keyboard End
#define Keyboard_PageDown         78  // Keyboard PageDown

#define Keyboard_RightArrow       79  // Keyboard RightArrow
#define Keyboard_LeftArrow        80  // Keyboard LeftArrow
#define Keyboard_DownArrow        81  // Keyboard DownArrow
#define Keyboard_UpArrow          82  // Keyboard UpArrow

#define Keypad_NumLock            83  // Keypad Num Lock and Clear
#define Keypad_DIV                84  // Keypad /
#define Keypad_MULT               85  // Keypad *
#define Keypad_SUB                86  // Keypad -
#define Keypad_ADD                87  // Keypad +
#define Keypad_ENTER              88  // Keypad ENTER
#define Keypad_1                  89  // Keypad 1 and End
#define Keypad_2                  90  // Keypad 2 and Down Arrow
#define Keypad_3                  91  // Keypad 3 and PageDn
#define Keypad_4                  92  // Keypad 4 and Left Arrow
#define Keypad_5                  93  // Keypad 5
#define Keypad_6                  94  // Keypad 6 and Right Arrow
#define Keypad_7                  95  // Keypad 7 and Home
#define Keypad_8                  96  // Keypad 8 and Up Arrow
#define Keypad_9                  97  // Keypad 9 and PageUp
#define Keypad_0                  98  // Keypad 0 and Insert
#define Keypad_DEC                99  // Keypad . and Delete

#define Keyboard_Apps             101 // Keyboard Application


#define Mediakey_mute             200
#define Mediakey_volp             201
#define Mediakey_voln             202
#define Mediakey_pause            203
#define Mediakey_stop             204
#define Mediakey_last             205
#define Mediakey_next             206


#define Keyboard_LeftControl      224
#define Keyboard_LeftShift        225
#define Keyboard_LeftAlt          226
#define Keyboard_LeftGUI          227
#define Keyboard_RightControl     228
#define Keyboard_RightShift       229
#define Keyboard_RightAlt         230
#define Keyboard_RightGUI         231



/*----------------------------*/


extern struct SysTime_REG SysTime;

//键位表
extern const uint8_t KeyValueDef[KEY_COL_NUM][KEY_ROW_NUM];
extern const uint8_t KeyFNValueDef[KEY_COL_NUM][KEY_ROW_NUM];

//按键键值
extern uint8_t key_fresh; //键值刷新
extern uint8_t key_buff[KEY_COL_NUM * KEY_ROW_NUM];
extern uint8_t key_buff2; //GUI
extern uint8_t key_buff3; //media

#define HID_EPIN_SIZE                 64
#define HID_EPIN_SIZE2                2
#define HID_EPOUT_SIZE        				8

extern uint8_t KeyboardOutData[HID_EPOUT_SIZE];
extern uint8_t KeyboardInData[HID_EPIN_SIZE];
extern uint8_t KeyboardInData2[HID_EPIN_SIZE2];

//同时保持的按键数量
extern uint8_t key_keep_num;

/*----------------------------*/

extern const uint32_t SPIData0[120];







#define USB_MODE 1
#define BLE_MODE 2

extern uint8_t SysState;


//不同模式发送的长度
#define USB_MODE_LEN 61
#define BLE_MODE_LEN 10



















#endif
