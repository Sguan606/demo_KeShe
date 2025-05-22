#ifndef __TEMP_H
#define __TEMP_H

#include "stm32f10x.h"
#include "AD.h"

float Get_RealTemp(void);
void Temp_Set(float temp);

#endif // TEMP_H
