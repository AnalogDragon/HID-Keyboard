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


//����
union LED_DATA_UNI LedData = {0};

//����buffer
static uint32_t SPIData[LED_NUM * 3];


extern SPI_HandleTypeDef hspi1;


void DispOnce(void){
  uint32_t temp0;
  
  //�Ƚ��б任
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
  
  //����SPI
  HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)SPIData, sizeof(SPIData));
  
}


//�ȼ�����
const uint8_t Hotkeys[4] = {12, 13, 14, 15,};

static uint8_t LED_ColorVect[6][21] = {0};

//��ɫ
static uint8_t LED_ColorRoot_Hue = 0;       //ɫ��
static uint8_t LED_ColorRoot_Sat = 0;       //������
//���ȼ���ɫ����ɫ��˫ɫ��Ҫ���ɫ��ʹ�����
static uint8_t LED_ColotRoot[22][3] = {0};  //ɫ��
//��������
static uint8_t LED_ColotBrght[22][3] = {0}; //ɫ�� * ����
//��ɫ��buff


static uint8_t LED_State = 1;         //LED����
static uint8_t LED_DispMode = 0;      //��ʾģʽ
static uint8_t LED_ColorMode = 0;     //��ɫģʽ
static uint8_t LED_HotkeyMode = 0;    //�ȼ�ģʽ



/*
��ɫ���ࣺ��ɫ / ˫ɫ / �ʺ�ɫ / ���彥��ɫ
�������ࣺ���� / ���� / ���� / ���� / ��ɢ / ������ֱ䵭 / ���水ť��ɢ / ���水���䵭

�ȼ���ʾ����ɫ / ��ɫ / ����
*/

/*-----------��ɫ-----------*/


//��ɫ����
void SingelColorMode(void){
}

//���䣬����
void FlowRightMode(void){
}

//���䣬����
void FlowLeftMode(void){
}

//���䣬����
void FlowUpMode(void){
}

//���䣬����
void FlowDownMode(void){
}

//���䣬��ɢ
void FlowMiddleMode(void){
}

//������ֻ����䵭
void RanMode(void){
}

//���水����ɢ
void KeyMiddleMode(void){
}

//���水����ɢ
void KeyMode(void){
}





enum{
  
 LED_CMD_NOTHING = 0,
 LED_CMD_CLOSE,
  
 LED_CMD_MODE_ADD,  //�ƹ�ģʽ
 LED_CMD_MODE_SUB,
  
 LED_CMD_LIGHT_ADD, //�ƹ�����
 LED_CMD_LIGHT_SUB,
  
 LED_CMD_COLOR_ADD, //��ɫ����
 LED_CMD_COLOR_SUB, //ɫ��
  
 LED_CMD_COLOR_UP,  //�Աȶ�
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














