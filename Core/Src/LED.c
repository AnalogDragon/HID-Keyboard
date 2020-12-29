#include "LED.h"
#include "string.h"



struct LED_DATA_DEF{
  uint8_t D1[LED_NUM];
  uint8_t D2[LED_NUM];
  uint8_t D3[LED_NUM];
};

struct LED_COLOR_DEF{
  uint8_t G[LED_NUM];
  uint8_t R[LED_NUM];
  uint8_t B[LED_NUM];
};

union LED_DATA_UNI{
  struct LED_DATA_DEF Out;
  struct LED_COLOR_DEF Color;
};


//操作
union LED_DATA_UNI LedData = {0};

//发送buffer
static uint32_t SPIData[LED_NUM * 3];


extern SPI_HandleTypeDef hspi1;


void DispOnce(void){
  uint32_t temp0;
  
  //先进行变换
  for(uint16_t i = 0; i < LED_NUM*3; i+=3){
    
    temp0 = OneWire_0 << 28 | OneWire_0 << 24 | OneWire_0 << 20 | OneWire_0 << 16 | OneWire_0 << 12 | OneWire_0 << 8 | OneWire_0 << 4 | OneWire_0;
    if(LedData.Out.D1[i] & 0x80)temp0 ^= OneWire_1_XOR << 4;
    if(LedData.Out.D1[i] & 0x40)temp0 ^= OneWire_1_XOR << 0;
    if(LedData.Out.D1[i] & 0x20)temp0 ^= OneWire_1_XOR << 12;
    if(LedData.Out.D1[i] & 0x10)temp0 ^= OneWire_1_XOR << 8;
    if(LedData.Out.D1[i] & 0x08)temp0 ^= OneWire_1_XOR << 20;
    if(LedData.Out.D1[i] & 0x04)temp0 ^= OneWire_1_XOR << 16;
    if(LedData.Out.D1[i] & 0x02)temp0 ^= OneWire_1_XOR << 28;
    if(LedData.Out.D1[i] & 0x01)temp0 ^= OneWire_1_XOR << 24;
    SPIData[i+0] = temp0;
    
    temp0 = OneWire_0 << 28 | OneWire_0 << 24 | OneWire_0 << 20 | OneWire_0 << 16 | OneWire_0 << 12 | OneWire_0 << 8 | OneWire_0 << 4 | OneWire_0;
    if(LedData.Out.D2[i] & 0x80)temp0 ^= OneWire_1_XOR << 4;
    if(LedData.Out.D2[i] & 0x40)temp0 ^= OneWire_1_XOR << 0;
    if(LedData.Out.D2[i] & 0x20)temp0 ^= OneWire_1_XOR << 12;
    if(LedData.Out.D2[i] & 0x10)temp0 ^= OneWire_1_XOR << 8;
    if(LedData.Out.D2[i] & 0x08)temp0 ^= OneWire_1_XOR << 20;
    if(LedData.Out.D2[i] & 0x04)temp0 ^= OneWire_1_XOR << 16;
    if(LedData.Out.D2[i] & 0x02)temp0 ^= OneWire_1_XOR << 28;
    if(LedData.Out.D2[i] & 0x01)temp0 ^= OneWire_1_XOR << 24;
    SPIData[i+1] = temp0;
    
    temp0 = OneWire_0 << 28 | OneWire_0 << 24 | OneWire_0 << 20 | OneWire_0 << 16 | OneWire_0 << 12 | OneWire_0 << 8 | OneWire_0 << 4 | OneWire_0;
    if(LedData.Out.D3[i] & 0x80)temp0 ^= OneWire_1_XOR << 4;
    if(LedData.Out.D3[i] & 0x40)temp0 ^= OneWire_1_XOR << 0;
    if(LedData.Out.D3[i] & 0x20)temp0 ^= OneWire_1_XOR << 12;
    if(LedData.Out.D3[i] & 0x10)temp0 ^= OneWire_1_XOR << 8;
    if(LedData.Out.D3[i] & 0x08)temp0 ^= OneWire_1_XOR << 20;
    if(LedData.Out.D3[i] & 0x04)temp0 ^= OneWire_1_XOR << 16;
    if(LedData.Out.D3[i] & 0x02)temp0 ^= OneWire_1_XOR << 28;
    if(LedData.Out.D3[i] & 0x01)temp0 ^= OneWire_1_XOR << 24;
    SPIData[i+2] = temp0;

  }
  
  //操作SPI
  HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)SPIData, sizeof(SPIData));
  
}


//热键区域
const uint8_t Hotkeys[4] = {12, 13, 14, 15,};

static uint8_t LED_ColorVect[6][21] = {0};

//根色
static uint8_t LED_ColorRoot_Hue = 0;       //色相
static uint8_t LED_ColorRoot_Sat = 0;       //纯净度
//首先计算色表，单色和双色需要借鉴色相和纯净度
static uint8_t LED_ColotRoot[22][3] = {0};  //色表
//加入亮度
static uint8_t LED_ColotBrght[22][3] = {0}; //色表 * 亮度
//填色到buff


static uint8_t LED_State = 1;         //LED开关
static uint8_t LED_DispMode = 0;      //显示模式
static uint8_t LED_ColorMode = 0;     //颜色模式
static uint8_t LED_HotkeyMode = 0;    //热键模式



/*
颜色分类：单色 / 双色 / 彩虹色 / 整体渐变色
动画分类：向左 / 向右 / 向上 / 向下 / 扩散 / 随机出现变淡 / 跟随按钮扩散 / 跟随按键变淡

热键显示：单色 / 反色 / 高亮
*/

/*-----------单色-----------*/


//单色不变
void SingelColorMode(void){
}

//渐变，向右
void FlowRightMode(void){
}

//渐变，向左
void FlowLeftMode(void){
}

//渐变，向上
void FlowUpMode(void){
}

//渐变，向下
void FlowDownMode(void){
}

//渐变，扩散
void FlowMiddleMode(void){
}

//随机出现缓慢变淡
void RanMode(void){
}

//跟随按键扩散
void KeyMiddleMode(void){
}

//跟随按键扩散
void KeyMode(void){
}





enum{
  
 LED_CMD_NOTHING = 0,
 LED_CMD_CLOSE,
  
 LED_CMD_MODE_ADD,  //灯光模式
 LED_CMD_MODE_SUB,
  
 LED_CMD_LIGHT_ADD, //灯光亮度
 LED_CMD_LIGHT_SUB,
  
 LED_CMD_COLOR_ADD, //颜色控制
 LED_CMD_COLOR_SUB, //色相
  
 LED_CMD_COLOR_UP,  //对比度
 LED_CMD_COLOR_DN,
  
}LED_CTL_DEF;



void LedCmd(uint8_t CMD){
  switch(CMD){
    case LED_CMD_CLOSE:
      break;
    
    default:
      break;
  }
}


void ColorMode(void){
  
}














