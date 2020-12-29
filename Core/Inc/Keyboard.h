#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "main.h"





#define KEY_COL_NUM           9
#define KEY_ROW_NUM           12



void SetKeyboardLED(uint8_t sta);
void ReadKeyCall(void);
void KeyFilter(uint8_t ColStep);

void GetKeyValue(uint8_t (*pKeyValueDef)[KEY_ROW_NUM]);
// void GetFnKeyValue(void);

void ResetKeyValue(void);


#endif


