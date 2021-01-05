#include "bat.h"
#include "string.h"


extern ADC_HandleTypeDef hadc1;

//����ֵ
static uint16_t ADC_BUFF[ADC_CH_NUM];
//��ֵ�˲��м����
static uint32_t ADC_ADD[ADC_CH_NUM];
//ADC���ֵ
static uint16_t ADC_OUT[ADC_CH_NUM];

static float ADC_Volt[ADC_CH_NUM];

//������ɱ��
static uint8_t ADCCpltFlag = 0;




//��ֵ�˲�
void ADCDataFilter(void){
  static uint16_t index = 0;
  
  if(index >= ADC_BUFF_LEN){  //����������ֵ
    //��ֵ
    ADC_OUT[0] = ADC_ADD[0] / ADC_BUFF_LEN;
    ADC_OUT[1] = ADC_ADD[1] / ADC_BUFF_LEN;
    ADC_OUT[2] = ADC_ADD[2] / ADC_BUFF_LEN;
    //�����ѹ
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

//��ȡADC��ֵ
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
      ADCDataFilter();  //���һ�κ�����˲�
      step = 0;
      break;
    
    default:
      step = 0;
      break;
      
  }
  
}


/*
  �����ص�����0-10
*/
uint8_t BatteryConv(void){
	uint8_t i = 0;
	static uint8_t PowerBak = 10;
  static float Volt = 0;
	
  //��ص�ѹ-������
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
  
  /*----���ͷ�ֹ��ֵ���ȶ�----*/
  
  //С���򽵵͵���
	for(i = 0; i < 10; i ++){
		if((Volt * 2) < table[i])
			break;
	}
	if(i < PowerBak){
		PowerBak = i;
	}
	
  //����0.005V�����ӵ���
	for(i = 0; i < 10; i ++){
		if((Volt * 2) < (table[i] + 0.005))
			break;
	}
	if(i > PowerBak){
		PowerBak = i;
	}
	
	return PowerBak;
}





