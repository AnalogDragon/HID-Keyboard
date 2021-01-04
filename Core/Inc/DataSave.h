#ifndef _DATASAVE_H_
#define _DATASAVE_H_
#include "main.h"



#define FLASH_BASE_ADDR     0x8000000
#define FLASH_FULL_SIZE     0x40000
#define FLASH_TAIL_ADDR     (FLASH_BASE_ADDR + FLASH_FULL_SIZE)

//´æ´¢Ò³µÄÊýÁ¿
#define FLASH_SAVE_PAGE     2
#define FLASH_SAVE_SIZE     (FLASH_SAVE_PAGE * FLASH_PAGE_SIZE)
#define FLASH_SAVE_ADDR     (FLASH_TAIL_ADDR - FLASH_SAVE_SIZE)

#define FLASH_ONCE_SIZE     ((sizeof(SaveData) + 1) / 2)



uint8_t SaveDataOnce(void);
void SaveDataInit(void);


#endif
