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
  UsartTxBuffer[0] = UART_HEAR_TX;  //head
  UsartTxBuffer[1] = UART_BLE_ADDR;  //address
  UsartTxBuffer[3] = BLE_CMD;     //CMD
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
  uint8_t Len;
  
  BLERxTime = SysTime.SysTimeCNT100ms;
  BLETxFlag = 0;
  
  Len = (huart1.pRxOutPtr[2] > sizeof(BleState.all)) ? sizeof(BleState.all) : huart1.pRxOutPtr[2];
  memcpy(BleState.all, &huart1.pRxOutPtr[3], Len);
  
  if(SysState.COM == BLE_MODE){
    
    if(BleState.bit.LinkSta != 0){
      //LED Data
      KeyboardOutData[0] = 1;
      KeyboardOutData[1] = BleState.bit.LEDState;
    }
    
    //重发
    if(BleState.bit.ReSendReq){
      if(huart1.gState == HAL_UART_STATE_READY){
        HAL_UART_Transmit_IT(&huart1, UsartTxBuffer, huart1.TxXferSize);
      }
    }
  }
}








