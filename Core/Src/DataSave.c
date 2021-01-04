#include "DataSave.h"
#include "string.h"

const uint16_t TestData0[20] = {
0x0101,0x0202,0x0303,0x0404,0x0505,0x0606,0x0707,0x0808,0x0909,0,0x0a0a,0x0b0b,0x0c0c,0x0d0d,0x0e0e,0x0f0f,0x0101,0x0202,0x0303,0x0404,};
const uint16_t TestData1[20] = {
0x1111,0x2222,0x3333,0x4444,0x5555,0x6666,0x7777,0x8888,0x9999,0,0xaaaa,0xbbbb,0xcccc,0xdddd,0xeeee,0xffff,0x1111,0x2222,0x3333,0x4444,};
  
uint16_t TestData2[20] = {0};

static FLASH_EraseInitTypeDef flashinit;
static uint32_t ErrCode;


//擦除保存区域
uint8_t SaveDataClear(void){
  flashinit.TypeErase = FLASH_TYPEERASE_PAGES;
  flashinit.PageAddress = FLASH_SAVE_ADDR;
  flashinit.NbPages = FLASH_SAVE_PAGE;
  
  if(HAL_OK == HAL_FLASHEx_Erase(&flashinit, &ErrCode))return HAL_OK;
  return HAL_ERROR;
}

//保存数据在末尾
uint8_t SaveDataWrite(uint32_t ADDR){
  for(uint32_t i = 0; i < FLASH_ONCE_SIZE; i ++){
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD ,ADDR + i * 2 ,SaveData.All[i]) != HAL_OK)return HAL_ERROR;
  }
  return HAL_OK;
}

//读取
void SaveDataRead(uint32_t ADDR){
  uint16_t *pData;
  for(uint32_t i = 0; i < FLASH_ONCE_SIZE; i ++){
    pData = (uint16_t*)(ADDR - i * 2);
    SaveData.All[(sizeof(SaveData.All) / 2) - (i + 1)] = *pData;
  }
}

//检测数据正确
uint8_t SaveDataCheck(uint32_t ADDR){
  uint16_t *pData;
  for(uint32_t i = 0; i < FLASH_ONCE_SIZE; i ++){
    pData = (uint16_t*)(ADDR - i * 2);
    if(SaveData.All[(sizeof(SaveData.All) / 2) - (i + 1)] != *pData)return HAL_ERROR;
  }
  return HAL_OK;
}


//寻找存储队尾
uint32_t FindSaveDataTail(void){
  uint32_t i;
  uint16_t *pData;
  for(i = 2; i < FLASH_SAVE_SIZE; i += 2){
    pData = (uint16_t*)(FLASH_TAIL_ADDR - i);
    if(*pData != 0xFFFF){
      return FLASH_TAIL_ADDR - i;
    }
  }
  return FLASH_SAVE_ADDR;
}


uint8_t SaveDataOnce(void){
  uint32_t addr = 0;
  uint8_t ErrTime = 0;
  
  //计算包数据
  SaveData.Data.Head = 0xCCCC;
  SaveData.Data.Len = (sizeof(struct SAVEDATA_DEF)+1)/2;
  SaveData.Data.Verify = CRCCheck(SaveData.Data.Data.All,sizeof(SaveData.Data.Data.All));
  
L1:
  //获取末尾地址
  addr = FindSaveDataTail() + 2;
  
  while(HAL_FLASH_Unlock() != HAL_OK);
  
  //存满，擦除flash
  if(FLASH_TAIL_ADDR - addr < 8 + FLASH_ONCE_SIZE){
    SaveDataClear();
    addr = FLASH_SAVE_ADDR;
  }
  
  //写flash
  SaveDataWrite(addr);
  
  while(HAL_FLASH_Lock() != HAL_OK);
  
  addr = FindSaveDataTail();
  
  //校验
  if(SaveDataCheck(addr) != HAL_OK){
    ErrTime ++;
    if(ErrTime > 3)return HAL_ERROR;
    goto L1;
  }
  
  return HAL_OK;
}


void SaveDataInit(void){
  uint32_t addr = 0;
  
  addr = FindSaveDataTail();
  
  //读取
  SaveDataRead(addr);
  
  if(SaveData.Data.Head != 0xCCCC
  || SaveData.Data.Len != (sizeof(struct SAVEDATA_DEF)+1)/2
  || SaveData.Data.Verify != CRCCheck(SaveData.Data.Data.All,sizeof(SaveData.Data.Data.All))){
    InitData();
    SaveDataOnce();
  }
  
}













