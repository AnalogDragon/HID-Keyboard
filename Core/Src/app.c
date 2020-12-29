#include "app.h"
#include "usb_device.h"
#include "string.h"



//1msϵͳʱ��
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


//��Դ����
void SetPower(uint8_t sta){
  PowerLock_IO = (sta != 0);
}

//����оƬ��Դ����
void SetPowerBLE(uint8_t sta){
  BLEPowerLock_IO = (sta != 0);
}


void KeyboardTask(void){
  static uint8_t ClearKeyboard = 0;
  static uint8_t KEY_FN_OUT = 0;
  static uint8_t KEY_FN_COUNT = 0;
  
  //FN���˲�
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
  
  //����FN����ͬ����ѯ����
  if(KEY_FN_OUT){ //������Ҫ������˲�
    
    //����FN�����ͨ����
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
    
    //��ȡ������ֵ
    GetKeyValue((uint8_t (*)[KEY_ROW_NUM])KeyFNValueDef);
    
  }
  else{
    
    //�˳�FN���FN����
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
    
    //��ȡFN��ֵ
    GetKeyValue((uint8_t (*)[KEY_ROW_NUM])KeyValueDef);
    
  }
  
  
  ////keyboard key send
  if((key_fresh & 1) != 0){
    key_fresh &= ~1;
    if(SysState == USB_MODE){
      KeyboardInData[0] = 1;  //set ID
      KeyboardInData[1] = key_buff2;  // key
      memcpy(&KeyboardInData[3],key_buff,key_keep_num); //��ֵ
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


//ģʽ�л�
void ModeChangeTask(void){
  
  if(SysState == USB_MODE){
    if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED){  //usb�Ͽ�
      memset(KeyboardOutData,0,sizeof(KeyboardOutData));
      SetKeyboardLED(0);
      SysState = BLE_MODE;
      key_keep_num = BLE_MODE_LEN;
    }
  }
  else if(SysState == BLE_MODE){
    if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){  //usb����
      SysState = USB_MODE;
      key_keep_num = USB_MODE_LEN;
    }
  }
  else{ //�쳣��λ
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




