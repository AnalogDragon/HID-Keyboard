#include "Keyboard.h"
#include "string.h"


//0：正常键盘
//1：12键键盘（maimai外设）
#define mode_keyboard       0


//配置输出LED
void SetKeyboardLED(uint8_t sta){
  LED1_IO = ((sta & 0x01) != 0);
  LED2_IO = ((sta & 0x02) != 0);
  LED3_IO = ((sta & 0x04) != 0);
}



//未用
#if 0
void ReadKeyCall(void){
  static uint8_t ColStep = 0;
  
  /**/
  ColStep++;
  if(ColStep >= KEY_COL_NUM){
    ColStep = 0;
  }
  
  /**/
  KeyDataNow[ColStep] = (KEY_IN12<<11)  | (KEY_IN11<<10)  | (KEY_IN10<<9)   | (KEY_IN9<<8)|
                        (KEY_IN8<<7)    | (KEY_IN7<<6)    | (KEY_IN6<<5)    | (KEY_IN5<<4)|
                        (KEY_IN4<<3)    | (KEY_IN3<<2)    | (KEY_IN2<<1)    | KEY_IN1;
  
  /*Filter 3 times*/
  KeyDataOut[ColStep] |= KeyDataLast1[ColStep] & KeyDataNow[ColStep] & KeyDataLast2[ColStep];  //
  KeyDataOut[ColStep] &= KeyDataLast1[ColStep] | KeyDataNow[ColStep] | KeyDataLast2[ColStep];  //
  KeyDataLast2[ColStep] = KeyDataLast1[ColStep]; //旧值转移
  KeyDataLast1[ColStep] = KeyDataNow[ColStep];
  
  /**/
  switch(ColStep){
    
    case 0:
      KEY_OUT1_IO = 1;
      KEY_OUT2_IO = 0;
      break;
    
    case 1:
      KEY_OUT2_IO = 1;
      KEY_OUT3_IO = 0;
      break;
    
    case 2:
      KEY_OUT3_IO = 1;
      KEY_OUT4_IO = 0;
      break;
    
    case 3:
      KEY_OUT4_IO = 1;
      KEY_OUT5_IO = 0;
      break;
    
    case 4:
      KEY_OUT5_IO = 1;
      KEY_OUT6_IO = 0;
      break;
    
    case 5:
      KEY_OUT6_IO = 1;
      KEY_OUT7_IO = 0;
      break;
    
    case 6:
      KEY_OUT7_IO = 1;
      KEY_OUT8_IO = 0;
      break;
    
    case 7:
      KEY_OUT8_IO = 1;
      KEY_OUT9_IO = 0;
      break;
    
    case 8:
      KEY_OUT9_IO = 1;
      KEY_OUT1_IO = 0;
      break;
    
    default:
      break;
    
  }
  
}
#endif





//判断ctrl等
uint8_t CheckKeyValue(uint8_t Val){
  const uint8_t KeyMin = Keyboard_LeftControl;
  const uint8_t KeyMax = Keyboard_RightGUI;
  
  if(Val >= KeyMin && Val <= KeyMax){
    return 1 << (Val-KeyMin);
  }
  return 0;
}


//判断media
uint8_t CheckKeyMediaValue(uint8_t Val){
  const uint8_t KeyMin = Mediakey_mute;
  const uint8_t KeyMax = Mediakey_next;
  
  if(Val >= KeyMin && Val <= KeyMax){
    return 1 << (Val-KeyMin);
  }
  return 0;
}


//判断BLE
uint8_t CheckKeyBLEValue(uint8_t Val){
  const uint8_t KeyMin = BLEK_Disconnect;
  const uint8_t KeyMax = BLEK_ClearBind;
  
  if(Val >= KeyMin && Val <= KeyMax){
    return Val;
  }
  return 0;
}



//增加一个值
void AddKeyValue(uint8_t Value){
  uint8_t i,temp;
  
  if(Value == 0){
    return;
  }
  
  temp = CheckKeyValue(Value);
  if(temp != 0){
    key_buff2 |= temp;
    key_fresh |= 1; //keyboard
    return;
  }
  
  temp = CheckKeyMediaValue(Value);
  if(temp != 0){
    key_buff3 |= temp;
    key_fresh |= 2; //media
    return;
  }
  
  temp = CheckKeyBLEValue(Value);
  if(temp != 0){
    if(BLE_CMD == 0){
      BLE_CMD = temp;
    }
    return;
  }
  
  
  for(i = 0; i < KEY_COL_NUM * KEY_ROW_NUM; i++){
    if(key_buff[i] == 0){
      key_buff[i] = Value;
      if(i < key_keep_num){
        key_fresh |= 1; //keyboard
      }
      return;
    }
  }
  
}

//减去一个值
void SubKeyValue(uint8_t Value){
  uint8_t i,temp;
  
  if(Value == 0){
    return;
  }
  
  temp = CheckKeyValue(Value);
  if(temp != 0){
    key_buff2 &= ~temp;
    key_fresh |= 1; //keyboard
    return;
  }
  
  temp = CheckKeyMediaValue(Value);
  if(temp != 0){
    key_buff3 &= ~temp;
    key_fresh |= 2; //media
    return;
  }
  
  temp = CheckKeyBLEValue(Value);
  if(temp != 0){
    if(BLE_CMD == temp){
      BLE_CMD = 0;  //BLE
    }
    return;
  }
  
  //从保持范围内减去
  for(i = 0; i < key_keep_num; i++){
    
    if(key_buff[i] == Value){
      key_buff[i] = key_buff[key_keep_num]; //取出一个待发送值，可以是0
      if(key_buff[key_keep_num] != 0){          //后面有数据，进行移动
        for(i = key_keep_num; i < (KEY_COL_NUM * KEY_ROW_NUM - 1); i++){
          key_buff[i] = key_buff[i+1];
          if(key_buff[i+1] == 0){               //判断尾
            break;
          }
        }
        key_buff[i] = 0;
      }
      key_fresh |= 1; //keyboard
      return;
    }
    
  }
  
  //缓存删除
  for(i = key_keep_num; i < KEY_COL_NUM * KEY_ROW_NUM; i++){
    if(key_buff[i] == Value){
        for(i = i; i < (KEY_COL_NUM * KEY_ROW_NUM - 1); i++){
          key_buff[i] = key_buff[i+1];
          if(key_buff[i+1] == 0){               //判断尾
            break;
          }
        }
        key_buff[i] = 0;
        return;
    }
    if(key_buff[i] == 0){
      return;
    }
  }
  
}


#if (mode_keyboard == 0)


//滤波长度调整
#define FILTERSEC  5
#define FILTERTIME  (FILTERSEC * 20 / 9)


static uint16_t KeyData[FILTERTIME][KEY_COL_NUM] = {0};
static uint16_t KeyDataOut[KEY_COL_NUM] = {0};
static uint16_t KeyDataBak[KEY_COL_NUM] = {0};



//刷新键值
//hal库浪费时间多，重写
/**
  * @brief This function handles TIM6 global interrupt.
  */
void TIM6_IRQHandler(void)
{
  static uint16_t ColStep = 0;
  static uint16_t addnum = 0;
  
    /* TIM Update event */
  if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)
  {
    if (__HAL_TIM_GET_IT_SOURCE(&htim6, TIM_IT_UPDATE) != RESET)
    {
      __HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
  
      /**/
      ColStep++;
      if(ColStep >= KEY_COL_NUM){
        ColStep = 0;
      }
      
      /**/
      KeyData[addnum][ColStep] = (KEY_IN12<<11)  | (KEY_IN11<<10)  | (KEY_IN10<<9)   | (KEY_IN9<<8)|
                                 (KEY_IN8<<7)    | (KEY_IN7<<6)    | (KEY_IN6<<5)    | (KEY_IN5<<4)|
                                 (KEY_IN4<<3)    | (KEY_IN3<<2)    | (KEY_IN2<<1)    | KEY_IN1;
      
      /**/
      switch(ColStep){
        
        case 0:
          KEY_OUT1_IO = 1;
          KEY_OUT2_IO = 0;
          break;
        
        case 1:
          KEY_OUT2_IO = 1;
          KEY_OUT3_IO = 0;
          break;
        
        case 2:
          KEY_OUT3_IO = 1;
          KEY_OUT4_IO = 0;
          break;
        
        case 3:
          KEY_OUT4_IO = 1;
          KEY_OUT5_IO = 0;
          break;
        
        case 4:
          KEY_OUT5_IO = 1;
          KEY_OUT6_IO = 0;
          break;
        
        case 5:
          KEY_OUT6_IO = 1;
          KEY_OUT7_IO = 0;
          break;
        
        case 6:
          KEY_OUT7_IO = 1;
          KEY_OUT8_IO = 0;
          break;
        
        case 7:
          KEY_OUT8_IO = 1;
          KEY_OUT9_IO = 0;
          break;
        
        case 8:
          KEY_OUT9_IO = 1;
          KEY_OUT1_IO = 0;
          addnum++;   //扫描最后一排时增加
          if(addnum >= FILTERTIME)addnum = 0;
          break;
        
        default:
          break;
        
      }
    }
  }
  
}




void GetKeyValue(uint8_t (*pKeyValueDef)[KEY_ROW_NUM]){
  uint8_t NoPressFlag = 0;
  static uint8_t PressedFlag = 0;
  static uint8_t key_keep_num_bak = 0;
  static uint8_t key_fresh_bak = 0;
  

  if(key_keep_num_bak != key_keep_num){
    key_keep_num_bak = key_keep_num;
    ResetKeyValue();
  }
  
  //计算滤波
  for(uint8_t j = 0; j < KEY_COL_NUM; j++){
    uint16_t temp1 = 0xFFFF;
    uint16_t temp2 = 0;
    for(uint16_t i = 0; i < FILTERTIME; i++){
      temp1 &= KeyData[i][j];
      temp2 |= KeyData[i][j];
    }
    KeyDataOut[j] |=  temp1;
    KeyDataOut[j] &=  temp2;
  }
  
  //检测按键按下
  for(uint8_t i = 0; i < KEY_COL_NUM; i++){
    uint16_t temp = KeyDataBak[i] ^ KeyDataOut[i];
    if(temp != 0){
      PressedFlag = 1;
      
      for(uint8_t j = 0; j < KEY_ROW_NUM; j++){
        if(((temp >> j) & 1) != 0){
          if(((KeyDataOut[i] >> j) & 1) != 0){  //按下
            KeyDataBak[i] |= 1<<j;
            AddKeyValue(pKeyValueDef[i][j]);
            key_fresh_bak |= key_fresh;
            if(key_fresh != 0 && SysState.COM == USB_MODE){ //按键插入了，退出发送hid
              return;
            }
          }
          else{//弹起
            KeyDataBak[i] &= ~(1<<j);
            SubKeyValue(pKeyValueDef[i][j]);
            key_fresh_bak |= key_fresh;
          }
        }
      }
    }
    if(KeyDataOut[i] != 0)NoPressFlag = 1;
  }
  
  //蓝牙模式 检测是否空余
  if(SysState.COM == BLE_MODE && key_buff[key_keep_num] != 0){
    for(uint8_t i = 0; i < key_keep_num; i++){
      if(key_buff[i] == 0){
        key_buff[i] = key_buff[key_keep_num];
        
        for(i = key_keep_num; i < (KEY_COL_NUM * KEY_ROW_NUM - 1); i++){
          key_buff[i] = key_buff[i+1];
          if(key_buff[i+1] == 0){               //判断尾
            break;
          }
        }
        key_buff[i] = 0;
        
        if(key_buff[key_keep_num] == 0){        //末尾无数据
          break;
        }
      }
    }
  }
  
  if(NoPressFlag == 0 && PressedFlag != 0){//按键全清，并且按过按键
    PressedFlag = 0;
    key_buff2 = 0;
    key_buff3 = 0;
    BLE_CMD = 0;
    memset(key_buff,0,sizeof(key_buff));
    key_fresh = key_fresh_bak;  //刷新更改过的值
    key_fresh_bak = 0;
  }
  
}



void ResetKeyValue(void){
  key_buff2 = 0;
  key_buff3 = 0;
  BLE_CMD = 0;
  memset(key_buff,0,sizeof(key_buff));
  memset(KeyDataBak,0,sizeof(KeyDataBak));
}



#else


/*后面是maimai单按键*/


//mai键位表
const uint8_t KeyMaiValueDef[KEY_ROW_NUM] = {
 Keypad_1, Keypad_2, Keypad_3, Keypad_4, 
 Keypad_5, Keypad_6, Keypad_7, Keypad_8, 
 Keypad_9, Keypad_0, Mediakey_volp, Mediakey_voln, 
};


const uint8_t KeyMaiFNValueDef[KEY_ROW_NUM] = {
 Keypad_1, Keypad_2, Keypad_3, Keypad_4, 
 Keypad_5, Keypad_6, Keypad_7, Keypad_8, 
 Keypad_9, Keypad_0, Keypad_DEC, Keypad_ADD, 
};


//滤波长度调整
#define FILTERSEC  1
#define FILTERTIME  (20 * FILTERSEC)


//按键缓存
static uint16_t KeyMaiData[FILTERTIME] = {0};
static uint16_t KeyMaiDataOut = 0;
static uint16_t KeyDataOutBak = 0;


//刷新键值
/**
  * @brief This function handles TIM6 global interrupt.
  */
void TIM6_IRQHandler(void)
{
  static uint16_t addnum = 0;
  
    /* TIM Update event */
  if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)
  {
    if (__HAL_TIM_GET_IT_SOURCE(&htim6, TIM_IT_UPDATE) != RESET)
    {
      __HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);

      /**/
      KeyMaiData[addnum] = (KEY_IN12<<11)  | (KEY_IN11<<10)  | (KEY_IN10<<9)   | (KEY_IN9<<8)|
                      (KEY_IN8<<7)    | (KEY_IN7<<6)    | (KEY_IN6<<5)    | (KEY_IN5<<4)|
                      (KEY_IN4<<3)    | (KEY_IN3<<2)    | (KEY_IN2<<1)    | KEY_IN1;
      addnum++;
      if(addnum >= FILTERTIME)addnum = 0;
      
    }
  }
  
}




void GetKeyValue(uint8_t (*pKeyValueDef)[KEY_ROW_NUM]){
  static uint8_t PressedFlag = 0;
  static uint8_t key_fresh_bak = 0;
  uint8_t* pKeyMaiValueDef = (uint8_t*)&KeyMaiValueDef;
  
  if(pKeyValueDef == (uint8_t(*)[KEY_ROW_NUM])KeyFNValueDef){
    pKeyMaiValueDef = (uint8_t*)KeyMaiFNValueDef;
  }
  
  uint16_t temp1 = 0xFFFF;
  uint16_t temp2 = 0;
  for(uint16_t i = 0; i < FILTERTIME; i++){
    temp1 &= KeyMaiData[i];
    temp2 |= KeyMaiData[i];
  }
  KeyMaiDataOut |=  temp1;
  KeyMaiDataOut &=  temp2;
  
  //检测按键按下
  uint16_t temp = KeyDataOutBak ^ KeyMaiDataOut;
  
  if(temp != 0){
    PressedFlag = 1;
    
    for(uint8_t j = 0; j < KEY_ROW_NUM; j++){
      if(((temp >> j) & 1) != 0){
        if(((KeyMaiDataOut >> j) & 1) != 0){  //按下
          KeyDataOutBak |= 1<<j;
          AddKeyValue(pKeyMaiValueDef[j]);
          key_fresh_bak |= key_fresh;
          if(key_fresh != 0 && SysState == USB_MODE){ //按键插入了，退出发送hid
            return;
          }
        }
        else{ //弹起
          KeyDataOutBak &= ~(1<<j);
          SubKeyValue(pKeyMaiValueDef[j]);
          key_fresh_bak |= key_fresh;
        }
      }
    }
  }
  
  //蓝牙模式 检测是否空余
  if(SysState == BLE_MODE && key_buff[key_keep_num] != 0){
    for(uint8_t i = 0; i < key_keep_num; i++){
      if(key_buff[i] == 0){
        key_buff[i] = key_buff[key_keep_num];
        
        for(i = key_keep_num; i < (KEY_COL_NUM * KEY_ROW_NUM - 1); i++){
          key_buff[i] = key_buff[i+1];
          if(key_buff[i+1] == 0){               //判断尾
            break;
          }
        }
        key_buff[i] = 0;
        
        if(key_buff[key_keep_num] == 0){        //末尾无数据
          break;
        }
      }
    }
  }

  if(KeyMaiDataOut == 0 && PressedFlag != 0){ //按键全清，并且按过按键
    PressedFlag = 0;
    key_buff2 = 0;
    key_buff3 = 0;
    BLE_CMD = 0;
    memset(key_buff,0,sizeof(key_buff));
    key_fresh = key_fresh_bak;
    key_fresh_bak = 0;
  }
  
}



void ResetKeyValue(void){
  key_buff2 = 0;
  key_buff3 = 0;
  BLE_CMD = 0;
  memset(key_buff,0,sizeof(key_buff));
  KeyDataOutBak = 0;
}



#endif



