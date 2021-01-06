#include "uart.h"
#include "string.h"

extern UART_HandleTypeDef huart1;


//发送BLE数据，CMD以及按键
void SendBLE(uint8_t mode){
  uint16_t temp;
  uint16_t Len;
  static uint8_t NeedSend = 0;
  
  if(NeedSend == 0 && mode == 0){ //无需要发送，且空闲
    if(GetDtTime(BLETxTime, SysTime.SysTimeCNT1ms) < 100){  //10Hz 心跳包
      return;
    }
  }
  
  if(huart1.gState != HAL_UART_STATE_READY){
    NeedSend = 1;
    return;
  }
  
  if(BLETxFlag != 0)
    BLEErrCnt++;
  else
    BLEErrCnt = 0;
  
  NeedSend = 0;
  if(BLE_CMD == 0)BLE_CMD_BAK = 0;
  if(BLE_CMD_BAK !=  BLE_CMD)temp = BLE_CMD & 0x0F;
  UsartTxBuffer[0] = UART_HEAR_TX;  //head
  UsartTxBuffer[1] = UART_BLE_ADDR;  //address
  UsartTxBuffer[3] = temp | (BatteryLevel << 4);     //BAT | CMD
  UsartTxBuffer[4] = key_buff3; //media
  UsartTxBuffer[5] = key_buff2; //GUI
  memcpy(&UsartTxBuffer[6], key_buff, BLE_MODE_LEN);
  for(Len = 6 + BLE_MODE_LEN; Len > 4 && UsartTxBuffer[Len-1] == 0; Len--);
  UsartTxBuffer[2] = Len-3; //media
  temp = CRCCheck(UsartTxBuffer, Len);
  UsartTxBuffer[Len++] = temp>>8;
  UsartTxBuffer[Len++] = temp;
  
  HAL_UART_Transmit_IT(&huart1, UsartTxBuffer, Len);
  
  //记录发送状态
  BLETxFlag = 1;
  BLETxTime = SysTime.SysTimeCNT1ms;
}



//获取BLE数据
void GetBLE(void){
  
  BLERxTime = SysTime.SysTimeCNT100ms;
  BLETxFlag = 0;
  
  for(uint8_t i=0;i<sizeof(BleState.all);i++){
    if(huart1.pRxOutPtr[2] > i)BleState.all[i] = huart1.pRxOutPtr[3+i];
    else BleState.all[i] = 0;
  }
  
  if(SysState.COM == BLE_MODE){
    
    if(BleState.bit.LinkSta != 0){
      //LED Data
      KeyboardOutData[0] = 1;
      KeyboardOutData[1] = BleState.bit.LEDState;
    }
    else{
      KeyboardOutData[0] = 1;
      KeyboardOutData[1] = 0;
    }
    
  }
}








