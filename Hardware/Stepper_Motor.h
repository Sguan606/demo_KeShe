#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

#include "stm32f10x.h"
#include "Timer.h"

void Stepper_Init(void);
void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//控制步进电机角度和角速度的代码实现
//（主函数可方便调用）
void StepperMotor_X_Contrl(int32_t Velocity,int32_t Step);
void Stepper_X_Task(void);
void StepperMotor_Y_Contrl(int32_t Velocity,int32_t Step);
void Stepper_Y_Task(void);
void StepperMotor_Z_Contrl(int32_t Velocity,int32_t Step);
void Stepper_Z_Task(void);

#endif // STEPPER_MOTOR_H
