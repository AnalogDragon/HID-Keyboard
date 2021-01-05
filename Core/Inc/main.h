/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
  
#include "VarDefine.h"
#include "Keyboard.h"
#include "app.h"
#include "LED.h"
#include "uart.h"
#include "DataSave.h"
#include "bat.h"
  
  
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


//电源
#define PowerLock_IO          PBout(9)
#define BLEPowerLock_IO       PBout(4)
#define PowerKey_IO           PBin(8)
#define PowerVBus_IO          PBin(7)
#define BatteryCharge_IO      PBin(6)


//键盘IO
#define KEY_OUT1_IO           PAout(8)
#define KEY_OUT2_IO           PCout(9)
#define KEY_OUT3_IO           PCout(8)
#define KEY_OUT4_IO           PCout(7)
#define KEY_OUT5_IO           PCout(6)
#define KEY_OUT6_IO           PBout(15)
#define KEY_OUT7_IO           PBout(14)
#define KEY_OUT8_IO           PBout(13)
#define KEY_OUT9_IO           PBout(12)

#define KEY_IN1_IO            PBin(11)
#define KEY_IN2_IO            PBin(10)
#define KEY_IN3_IO            PBin(2)
#define KEY_IN4_IO            PBin(1)
#define KEY_IN5_IO            PBin(0)
#define KEY_IN6_IO            PCin(5)
#define KEY_IN7_IO            PCin(4)
#define KEY_IN8_IO            PCin(10)
#define KEY_IN9_IO            PCin(11)
#define KEY_IN10_IO           PCin(12)
#define KEY_IN11_IO           PDin(2)
#define KEY_IN12_IO           PBin(3)

#define KEY_IN1               (KEY_IN1_IO == 0)
#define KEY_IN2               (KEY_IN2_IO == 0)
#define KEY_IN3               (KEY_IN3_IO == 0)
#define KEY_IN4               (KEY_IN4_IO == 0)
#define KEY_IN5               (KEY_IN5_IO == 0)
#define KEY_IN6               (KEY_IN6_IO == 0)
#define KEY_IN7               (KEY_IN7_IO == 0)
#define KEY_IN8               (KEY_IN8_IO == 0)
#define KEY_IN9               (KEY_IN9_IO == 0)
#define KEY_IN10              (KEY_IN10_IO == 0)
#define KEY_IN11              (KEY_IN11_IO == 0)
#define KEY_IN12              (KEY_IN12_IO == 0)

//other
#define FIN_IO                PCin(3)

#define LED1_IO               PCout(0)
#define LED2_IO               PCout(1)
#define LED3_IO               PCout(2)


#define KEY_FN                (FIN_IO == 0)





extern TIM_HandleTypeDef htim6;

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
