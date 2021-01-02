#ifndef _APP_H_
#define _APP_H_
#include "main.h"


void SysTimeCall(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


void SetPower(uint8_t sta);
void SetPowerBLE(uint8_t sta);
uint16_t CRCCheck(uint8_t *pData, uint16_t Size);
uint16_t GetDtTime(uint16_t TimeBuf,uint16_t TimeBase);


void KeyboardTask(void);
void ModeChangeTask(void);
void KeyboardLedTask(void);
void BackLedTask(void);
void UartRecTask(void);



#endif


