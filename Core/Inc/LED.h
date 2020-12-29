#ifndef _LED_H_
#define _LED_H_
#include "main.h"





#define OneWire_0   7
#define OneWire_1   3
#define OneWire_1_XOR   (OneWire_1 ^ OneWire_0)

#define LED_NUM   (21 * 6)






void DispOnce(void);










#endif
