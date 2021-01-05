#include "bat.h"
#include "string.h"


extern ADC_HandleTypeDef hadc1;

//采样值
static uint16_t ADC_BUFF[ADC_CH_NUM];
//均值滤波中间变量
static uint32_t ADC_ADD[ADC_CH_NUM];
//ADC输出值
static uint16_t ADC_OUT[ADC_CH_NUM];

static float ADC_Volt[ADC_CH_NUM];

//采样完成标记
static uint8_t ADCCpltFlag = 0;




//均值滤波
void ADCDataFilter(void){
  static uint16_t index = 0;
  
  if(index >= ADC_BUFF_LEN){  //存满后计算均值
    //均值
    ADC_OUT[0] = ADC_ADD[0] / ADC_BUFF_LEN;
    ADC_OUT[1] = ADC_ADD[1] / ADC_BUFF_LEN;
    ADC_OUT[2] = ADC_ADD[2] / ADC_BUFF_LEN;
    //计算电压
    ADC_Volt[0] = ADC_OUT[0] * 1.2 / ADC_OUT[2];
    ADC_Volt[1] = ADC_OUT[1] * 1.2 / ADC_OUT[2];
    
    memset(ADC_ADD ,0 ,sizeof(ADC_ADD));
    index = 0;
    ADCCpltFlag = 1;
  }
  
  ADC_ADD[0] += ADC_BUFF[0];
  ADC_ADD[1] += ADC_BUFF[1];
  ADC_ADD[2] += ADC_BUFF[2];
  
  index++;
}

//获取ADC的值
void ADCGet(void){
  static uint8_t step = 0;
  
  switch(step){
    case 0:
      HAL_ADC_Start(&hadc1);
      HAL_ADC_PollForConversion(&hadc1,0);  
      step++;
      break;
    
    case 1:
    case 2:
      ADC_BUFF[step - 1] = HAL_ADC_GetValue(&hadc1);
      HAL_ADC_Start(&hadc1);
      HAL_ADC_PollForConversion(&hadc1,0);  
      step++;
      break;
    
    case 3:
      ADC_BUFF[step - 1] = HAL_ADC_GetValue(&hadc1);
      HAL_ADC_Stop(&hadc1);
      ADCDataFilter();  //完成一次后进行滤波
      step = 0;
      break;
    
    default:
      step = 0;
      break;
      
  }
  
}


/*
  输出电池电量，0-10
*/
uint8_t BatteryConv(void){
	uint8_t i = 0;
	static uint8_t PowerBak = 10;
  static float Volt = 0;
	
  //电池电压-电量表
	const float table[10] = {
	3.68,	//10%
	3.74,	//20%
	3.77,	//30%
	3.79,	//40%
	3.82,	//50%
	3.87,	//60%
	3.92,	//70%
	3.98,	//80%
	4.06,	//90%
	4.16,	//100%
	};
	
  ADCGet();
  
  if(ADCCpltFlag == 0)return PowerBak;
  ADCCpltFlag = 0;
  
  if(Volt == 0){
    Volt = ADC_Volt[0];
  }
  else{
    Volt = Volt*0.8 + ADC_Volt[0]*0.2;
  }
  
  /*----迟滞防止数值不稳定----*/
  
  //小于则降低电量
	for(i = 0; i < 10; i ++){
		if((Volt * 2) < table[i])
			break;
	}
	if(i < PowerBak){
		PowerBak = i;
	}
	
  //高于0.005V则增加电量
	for(i = 0; i < 10; i ++){
		if((Volt * 2) < (table[i] + 0.005))
			break;
	}
	if(i > PowerBak){
		PowerBak = i;
	}
	
	return PowerBak;
}





