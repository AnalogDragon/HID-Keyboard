#include "app.h"
#include "usb_device.h"
#include "string.h"



//1ms系统时钟
void SysTimeCall(void){
	
  SysTime.SysTimeCNT1ms++;
  SysTime.SysTimeFLG1ms = 1;
  
  if((SysTime.SysTimeCNT1ms%10) == 0){
    if(SysTime.SysTimeCNT1ms>=60000)SysTime.SysTimeCNT1ms = 0;
    SysTime.SysTimeCNT10ms++;
    SysTime.SysTimeFLG10ms = 1;
    
    if((SysTime.SysTimeCNT10ms%10) == 0){
      if(SysTime.SysTimeCNT10ms>=60000)SysTime.SysTimeCNT10ms = 0;
      SysTime.SysTimeCNT100ms++;
      SysTime.SysTimeFLG100ms = 1;
      
      if((SysTime.SysTimeCNT100ms%10) == 0){
        if(SysTime.SysTimeCNT100ms>=60000)SysTime.SysTimeCNT100ms = 0;
        SysTime.SysTimeCNT1s++;
        SysTime.SysTimeFLG1s = 1;
        
        if((SysTime.SysTimeCNT1s%60) == 0){
          if(SysTime.SysTimeCNT1s>=60000)SysTime.SysTimeCNT1s = 0;
          SysTime.SysTimeCNT1min++;
          SysTime.SysTimeFLG1min = 1;
          
          if((SysTime.SysTimeCNT1min%60) == 0){
            if(SysTime.SysTimeCNT1min>=60000)SysTime.SysTimeCNT1min = 0;
            SysTime.SysTimeCNT1h++;
            SysTime.SysTimeFLG1h = 1;
          }
        }
      }
    }
  }
}


//电源保持
void SetPower(uint8_t sta){
  PowerLock_IO = (sta != 0);
}

//蓝牙芯片电源开启
void SetPowerBLE(uint8_t sta){
  BLEPowerLock_IO = (sta != 0);
}


void KeyboardTask(void){
  static uint8_t ClearKeyboard = 0;
  static uint8_t KEY_FN_OUT = 0;
  static uint8_t KEY_FN_COUNT = 0;
  
  //FN键滤波
  if(KEY_FN != 0){
    if(KEY_FN_OUT == 0){
      KEY_FN_COUNT++;
      if(KEY_FN_COUNT > 10){
        KEY_FN_OUT = 1;
        KEY_FN_COUNT = 0;
      }
    }
  }
  else{
    if(KEY_FN_OUT != 0){
      KEY_FN_COUNT++;
      if(KEY_FN_COUNT > 10){
        KEY_FN_OUT = 0;
        KEY_FN_COUNT = 0;
      }
    }
  }
  
  //根据FN键不同，查询键表
  if(KEY_FN_OUT){ //后续需要加软件滤波
    
    //进入FN清除普通输入
    if(ClearKeyboard == 0){ 
      ClearKeyboard = 1;
      ResetKeyValue();
      if(SysState == USB_MODE){
        memset(KeyboardInData,0,sizeof(KeyboardInData));
        KeyboardInData[0] = 1;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE));
        KeyboardInData[0] = 2;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE2));
      }
      else{//uart send
      }
    }
    
    //获取正常键值
    GetKeyValue((uint8_t (*)[KEY_ROW_NUM])KeyFNValueDef);
    
  }
  else{
    
    //退出FN清除FN按键
    if(ClearKeyboard == 1){
      ClearKeyboard = 0;
      ResetKeyValue();
      if(SysState == USB_MODE){
        memset(KeyboardInData,0,sizeof(KeyboardInData));
        KeyboardInData[0] = 1;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE));
        KeyboardInData[0] = 2;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE2));
      }
      else{//uart send
      }
    }
    
    //获取FN键值
    GetKeyValue((uint8_t (*)[KEY_ROW_NUM])KeyValueDef);
    
  }
  
  
  ////keyboard key send
  if((key_fresh & 1) != 0){
    key_fresh &= ~1;
    if(SysState == USB_MODE){
      KeyboardInData[0] = 1;  //set ID
      KeyboardInData[1] = key_buff2;  // key
      memcpy(&KeyboardInData[3],key_buff,key_keep_num); //键值
      while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE));
    }
    else{//uart send
    }
  }
  
  ////media key send
  if((key_fresh & 2) != 0){
    key_fresh &= ~2;
    if(SysState == USB_MODE){
      KeyboardInData[0] = 2;  //set ID
      KeyboardInData[1] = key_buff3;  // key
      while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE2));
    }
    else{//uart send
    }
  }
  
  
}


//模式切换
void ModeChangeTask(void){
  
  if(SysState == USB_MODE){
    if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED){  //usb断开
      memset(KeyboardOutData,0,sizeof(KeyboardOutData));
      SetKeyboardLED(0);
      SysState = BLE_MODE;
      key_keep_num = BLE_MODE_LEN;
    }
  }
  else if(SysState == BLE_MODE){
    if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){  //usb连接
      SysState = USB_MODE;
      key_keep_num = USB_MODE_LEN;
    }
  }
  else{ //异常复位
    HAL_NVIC_SystemReset();
  }
  
}


void KeyboardLedTask(void){
  static uint8_t LED_STA = 0;
  if(KeyboardOutData[0] == 1){
    if(KeyboardOutData[1] != LED_STA){
      LED_STA = KeyboardOutData[1];
      SetKeyboardLED(LED_STA);
    }
  }
}




