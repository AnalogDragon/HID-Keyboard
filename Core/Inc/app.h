#ifndef _APP_H_
#define _APP_H_
#include "main.h"


void SysTimeCall(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


void SetPower(uint8_t sta);
void SetPowerBLE(uint8_t sta);
uint16_t CRCCheck(uint8_t *pData, uint16_t size);


void KeyboardTask(void);
void ModeChangeTask(void);
void KeyboardLedTask(void);
void BackLedTask(void);
void UsartTask(void);



#endif


