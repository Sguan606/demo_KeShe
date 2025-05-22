#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include "Temp.h"
#include "Stepper_Motor.h"

float Temp_Calculate(float target_temp,float real_temp);
void Pid_Control_Task(void);


#endif // PID_H
