#include "app.h"
#include "usb_device.h"
#include "string.h"

extern UART_HandleTypeDef huart1;


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


void Shutdown(void){
  SetPowerBLE(0);
  SetPower(0);
  HAL_NVIC_SystemReset();
  while(1);
}

//计算CRC
uint16_t CRCCheck(uint8_t *pData, uint16_t Size){
  uint16_t crc_result = 0xFFFF;
  
  for(uint16_t i=0;i<Size;i++)
  {
      crc_result ^= pData[i];
      for(uint8_t j=0;j<8;j++)
      {
          if(crc_result&0x01)
              crc_result=(crc_result>>1)^0xa001;
          else
              crc_result=crc_result>>1;
      }
  }
  return crc_result;
}

//获取时间差
uint16_t GetDtTime(uint16_t TimeBuf,uint16_t TimeBase){
  if(TimeBuf > TimeBase)
    return 60000 - TimeBuf + TimeBase;
  else
    return TimeBase - TimeBuf;
}


//键盘键值发送/更新
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
    else
      KEY_FN_COUNT = 0;
  }
  else{
    if(KEY_FN_OUT != 0){
      KEY_FN_COUNT++;
      if(KEY_FN_COUNT > 10){
        KEY_FN_OUT = 0;
        KEY_FN_COUNT = 0;
      }
    }
    else
      KEY_FN_COUNT = 0;
  }
  
  //根据FN键不同，查询键表
  if(KEY_FN_OUT){ //后续需要加软件滤波
    
    //进入FN清除普通输入
    if(ClearKeyboard == 0){
      ClearKeyboard = 1;
      ResetKeyValue();
      if(SysState.COM == USB_MODE){
        memset(KeyboardInData,0,sizeof(KeyboardInData));
        KeyboardInData[0] = 1;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE));
        KeyboardInData[0] = 2;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE2));
      }
    }
    
    //获取FN键值
    GetKeyValue((uint8_t (*)[KEY_ROW_NUM])KeyFNValueDef);
    
  }
  else{
    
    //退出FN清除FN按键
    if(ClearKeyboard == 1){
      ClearKeyboard = 0;
      ResetKeyValue();
      if(SysState.COM == USB_MODE){
        memset(KeyboardInData,0,sizeof(KeyboardInData));
        KeyboardInData[0] = 1;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE));
        KeyboardInData[0] = 2;  //set ID
        while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE2));
      }
    }
    
    //获取正常键值
    GetKeyValue((uint8_t (*)[KEY_ROW_NUM])KeyValueDef);
    
  }
  
  if(SysState.COM == USB_MODE){
    
    ////keyboard key send
    if((key_fresh & 1) != 0){
      key_fresh &= ~1;
      KeyboardInData[0] = 1;  //set ID
      KeyboardInData[1] = key_buff2;  // key
      memcpy(&KeyboardInData[3],key_buff,key_keep_num); //键值
      while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE));
    }
    
    ////media key send
    if((key_fresh & 2) != 0){
      key_fresh &= ~2;
      KeyboardInData[0] = 2;  //set ID
      KeyboardInData[1] = key_buff3;  // key
      while(USBD_OK != USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)KeyboardInData,HID_EPIN_SIZE2));
    }
    
  }
  else{ //BLE
    if(SysState.BLE != 0){
      SendBLE(key_fresh);
      key_fresh = 0;
    }
  }
  
}


//模式切换
void ModeChangeTask(void){
  
  if(SysState.COM == USB_MODE){
    key_keep_num = USB_MODE_LEN;
    if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED){  //usb断开
      memset(KeyboardOutData,0,sizeof(KeyboardOutData));
      SetKeyboardLED(0);
      SysState.COM = BLE_MODE;
      key_keep_num = BLE_MODE_LEN;
    }
  }
  else if(SysState.COM == BLE_MODE){
    key_keep_num = BLE_MODE_LEN;
    if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){  //usb连接
      SysState.COM = USB_MODE;
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


void BackLedTask(void){
  DispOnce();
}



//串口通信响应
void UartRecTask(void){
  uint16_t temp;
  
  if(huart1.RxDataSize != 0){
    
    if(huart1.pRxOutPtr[0] == UART_HEAR_RX && huart1.RxDataSize == (huart1.pRxOutPtr[2] + 5)){  //帧头
      temp = CRCCheck(huart1.pRxOutPtr, huart1.RxDataSize-2);
      temp ^= ((uint16_t)huart1.pRxOutPtr[huart1.RxDataSize-2] << 8) | huart1.pRxOutPtr[huart1.RxDataSize-1];
      
      if(temp == 0){  //CRC Verify
        
        switch(huart1.pRxOutPtr[1]){
          
          case UART_BLE_ADDR:
            //BLE模块的回复
            GetBLE();
            break;
          
          default:
            break;
          
        } //end of switch
      }//end of verify
      else{   //接收失败
        if(huart1.gState == HAL_UART_STATE_READY){
          HAL_UART_Transmit_IT(&huart1, UsartTxBuffer, huart1.TxXferSize);
        }
      }
      
    }
    else{
      if(huart1.gState == HAL_UART_STATE_READY){
        HAL_UART_Transmit_IT(&huart1, UsartTxBuffer, huart1.TxXferSize);
      }
    }
    huart1.RxDataSize = 0;
  }//end of data get
  
}

//BLE模块操作函数
void BLEMonitorTask(void){
  static uint8_t step = 0;
  static uint16_t time;
  static uint8_t flag;
  
  //切出BLE模式
  if(SysState.COM != BLE_MODE){
    if(flag){
      memset(BleState.all, 0, sizeof(BleState.all));
      SysState.BLE = 0;
      SetPowerBLE(0);
      step = 0;
    }
    return;
  }
  
  switch(step){
    
    case 0:
      //power on
      flag = 1;
      SysState.BLE = 0;
      SetPowerBLE(1);
      time = SysTime.SysTimeCNT10ms;
      step ++;
      break;
      
    case 1:
      //wait power on 1s
      if(GetDtTime(time, SysTime.SysTimeCNT10ms) > 100){
        time = SysTime.SysTimeCNT10ms;
        memset(BleState.all, 0, sizeof(BleState.all));
        SysState.BLE = 1;
        step ++;
      }
      break;
      
    case 2:
      //wait uart comm 0.5s
      if(GetDtTime(time, SysTime.SysTimeCNT10ms) > 50){
        BLEErrCnt = 0;
        step ++;
      }
      break;
      
    case 3:
      //idle
      if(BLEErrCnt > 20 || BleState.bit.NeedReset){
        //20次数据包丢失
        memset(KeyboardOutData,0,sizeof(KeyboardOutData));
        memset(BleState.all, 0, sizeof(BleState.all));
        SysState.BLE = 0;
        SetPowerBLE(0);
        SetKeyboardLED(0);
        time = SysTime.SysTimeCNT10ms;
        step ++;
        BLE_CMD_BAK = BLE_CMD;
      }
      if(BleState.bit.Timeout){
        Shutdown();
      }
      break;
      
    case 4:
      //wait power off 0.5s
      if(GetDtTime(time, SysTime.SysTimeCNT10ms) > 50){
        step = 0;
      }
      break;
      
    default:
      SysState.BLE = 0;
      SetPowerBLE(0);
      time = SysTime.SysTimeCNT10ms;
      step = 4;
      break;
  }
}



void SaveDataTask(void){
  static uint8_t NeedtoSave_Bak = 0;
  static uint16_t time;
  
  if(NeedtoSave != NeedtoSave_Bak){
    NeedtoSave_Bak = NeedtoSave;
    time = SysTime.SysTimeCNT100ms;
  }
  else{
    //5秒后保存数据
    if(NeedtoSave != 0 && GetDtTime(time, SysTime.SysTimeCNT100ms) > 50){
      NeedtoSave = 0;
      NeedtoSave_Bak = 0;
      if(SaveDataOnce() != HAL_OK){
        /*存储出错*/
      }
    }
  }
}


void BatteryTask(void){
  BatteryLevel = BatteryConv();
}


void InitData(void){
  memset(SaveData.All, 0, sizeof(SaveData.All));
  SaveData.Data.Data.Data.Key.Color = 0;
  SaveData.Data.Data.Data.Key.Flame = 1;
  SaveData.Data.Data.Data.Key.HotKey = 0;
}









