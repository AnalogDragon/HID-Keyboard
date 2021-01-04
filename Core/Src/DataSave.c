#include "DataSave.h"
#include "string.h"




static FLASH_EraseInitTypeDef flashinit;
static uint32_t ErrCode;


//������������
uint8_t SaveDataClear(void){
  flashinit.TypeErase = FLASH_TYPEERASE_PAGES;
  flashinit.PageAddress = FLASH_SAVE_ADDR;
  flashinit.NbPages = FLASH_SAVE_PAGE;
  
  if(HAL_OK == HAL_FLASHEx_Erase(&flashinit, &ErrCode))return HAL_OK;
  return HAL_ERROR;
}

//����������ĩβ
uint8_t SaveDataWrite(uint32_t ADDR){
  for(uint32_t i = 0; i < FLASH_ONCE_SIZE; i ++){
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD ,ADDR + i * 2 ,SaveData.All[i]) != HAL_OK)return HAL_ERROR;
  }
  return HAL_OK;
}

//��ȡ
void SaveDataRead(uint32_t ADDR){
  uint16_t *pData;
  for(uint32_t i = 0; i < FLASH_ONCE_SIZE; i ++){
    pData = (uint16_t*)(ADDR - i * 2);
    SaveData.All[(sizeof(SaveData.All) / 2) - (i + 1)] = *pData;
  }
}

//���������ȷ
uint8_t SaveDataCheck(uint32_t ADDR){
  uint16_t *pData;
  if(ADDR < FLASH_SAVE_ADDR + FLASH_ONCE_SIZE)
    return HAL_ERROR;
  for(uint32_t i = 0; i < FLASH_ONCE_SIZE; i ++){
    pData = (uint16_t*)(ADDR - i * 2);
    if(SaveData.All[(sizeof(SaveData.All) / 2) - (i + 1)] != *pData)return HAL_ERROR;
  }
  return HAL_OK;
}


//Ѱ�Ҵ洢��β
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


/*
  �洢����
  �洢ǰ�����ȼ���Ƿ�һ��
*/
uint8_t SaveDataOnce(void){
  uint32_t addr = 0;
  uint8_t ErrTime = 0;
  
L1:
  //��ȡĩβ��ַ
  addr = FindSaveDataTail();
  
  //����һ���򲻱���
  if(SaveDataCheck(addr) == HAL_OK){
    return HAL_OK;
  }
  
  //������ṹ
  SaveData.Data.Head = 0xCCCC;
  SaveData.Data.Verify = CRCCheck(SaveData.Data.Data.All,sizeof(SaveData.Data.Data.All));
  
  /*-----��ʼ����flash-----*/
  while(HAL_FLASH_Unlock() != HAL_OK);
  
  //����������flash
  if(FLASH_TAIL_ADDR - addr < 8 + FLASH_ONCE_SIZE){
    SaveDataClear();
    addr = FLASH_SAVE_ADDR;
  }
  else if(addr > FLASH_SAVE_ADDR){
    addr += 2;  //���׵�ַ����ƫ16b��ʼд��
  }
  
  //дflash
  SaveDataWrite(addr);
  
  while(HAL_FLASH_Lock() != HAL_OK);
  /*-----����-----*/
  
  //У��
  addr = FindSaveDataTail();
  if(SaveDataCheck(addr) != HAL_OK){
    ErrTime ++;
    if(ErrTime > 3)return HAL_ERROR;
    goto L1;
  }
  
  return HAL_OK;
}


/*
  ���ݳ�ʼ��
  ��ȡ�����¼�
*/
void SaveDataInit(void){
  uint32_t addr = 0;
  
  addr = FindSaveDataTail();
  
  //��ȡ
  SaveDataRead(addr);
  
  if(SaveData.Data.Head != 0xCCCC
  || SaveData.Data.Verify != CRCCheck(SaveData.Data.Data.All,sizeof(SaveData.Data.Data.All))){
    InitData();
    SaveDataOnce();
  }
  
}













