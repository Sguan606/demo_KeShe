/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-21 00:54:18
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-05-21 15:52:42
 * @FilePath: \demo_KeShe\Hardware\Stepper_Motor.c
 * @Description: [标准库]使用3个A4988驱动3个步进电机做XYZ轴运动;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Stepper_Motor.h"
#include <stdlib.h>  // 包含 abs() 函数的头文件

#define Motor_X_DIR GPIO_Pin_10
#define Motor_X_DIR_Port GPIOB
#define Motor_X_Speed GPIO_Pin_11
#define Motor_X_Speed_Port GPIOB

#define Motor_Y_DIR GPIO_Pin_12
#define Motor_Y_DIR_Port GPIOB
#define Motor_Y_Speed GPIO_Pin_13
#define Motor_Y_Speed_Port GPIOB

#define Motor_Z_DIR GPIO_Pin_14
#define Motor_Z_DIR_Port GPIOB
#define Motor_Z_Speed GPIO_Pin_15
#define Motor_Z_Speed_Port GPIOB

static uint32_t temp_X;
static uint32_t freq_count_X;
static int32_t Step_count_X;

static uint32_t temp_Y;
static uint32_t freq_count_Y;
static int32_t Step_count_Y;

static uint32_t temp_Z;
static uint32_t freq_count_Z;
static int32_t Step_count_Z;

void Stepper_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // 使能 GPIOA 时钟（F1 系列）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 XYZ 3轴的步进电机初始化 为推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/**
 * @description: 简单实现GPIO反转电平，发送脉冲的函数
 * @param {GPIO_TypeDef*} GPIOx
 * @param {uint16_t} GPIO_Pin
 * @return {*}
 */
void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    if (GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin) == Bit_SET) {
        GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);  // 高电平 → 低电平
    } else {
        GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);    // 低电平 → 高电平
    }
}

/**
 * @description: 设置X轴步进电机，转速百分比,Step为0则不设置具体步数;
 * @param {int32_t} Velocity 速度大小设置(包含正负)
 * @param {void} Step 给定具体转动步数
 * @return {*}
 */
void StepperMotor_X_Contrl(int32_t Velocity,int32_t Step)
{
    //需要设置10us定时中断一次
    __disable_irq();  // 关闭中断
    GPIO_WriteBit(Motor_X_DIR_Port, Motor_X_DIR, (Velocity > 0) ? Bit_SET : Bit_RESET);
    temp_X = (uint32_t)abs(Velocity);
    if (Step != 0){
        Step_count_X = (int32_t)Step;
    }
    else{
        Step_count_X = -1;
    }
    __enable_irq();   // 重新启用中断
}


/**
 * @description: X轴步进电机实时调控函数(已放入"Pid.c"中的TIM2_IRQHandler定时中断函数里面);
 * @return {*}
 */
void Stepper_X_Task(void)
{
    if (Step_count_X == -1)
    {
        freq_count_X += 1;
        // 在这里写你的定时任务代码
        if (freq_count_X == 10000/temp_X){
            GPIO_TogglePin(Motor_X_Speed_Port,Motor_X_Speed);
            freq_count_X = 0;
        }
    }
    else if(Step_count_X != 0)
    {
        freq_count_X += 1;
        // 在这里写你的定时任务代码
        if (freq_count_X == 10000/temp_X){
            GPIO_TogglePin(Motor_X_Speed_Port,Motor_X_Speed);
            freq_count_X = 0;
            Step_count_X--;
        }
    }
}
/**
 * @description: 设置X轴步进电机，转速百分比,Step为0则不设置具体步数;
 * @param {int32_t} Velocity 速度大小设置(包含正负)
 * @param {void} Step 给定具体转动步数
 * @return {*}
 */
void StepperMotor_Y_Contrl(int32_t Velocity,int32_t Step)
{
    //需要设置10us定时中断一次
    __disable_irq();  // 关闭中断
    GPIO_WriteBit(Motor_Y_DIR_Port, Motor_Y_DIR, (Velocity > 0) ? Bit_SET : Bit_RESET);
    temp_Y = (uint32_t)abs(Velocity);
    if (Step != 0){
        Step_count_Y = (int32_t)Step;
    }
    else{
        Step_count_Y = -1;
    }
    __enable_irq();   // 重新启用中断
}


/**
 * @description: X轴步进电机实时调控函数(已放入"Pid.c"中的TIM2_IRQHandler定时中断函数里面);
 * @return {*}
 */
void Stepper_Y_Task(void)
{
    if (Step_count_Y == -1)
    {
        freq_count_Y += 1;
        // 在这里写你的定时任务代码
        if (freq_count_Y == 10000/temp_Y){
            GPIO_TogglePin(Motor_Y_Speed_Port,Motor_Y_Speed);
            freq_count_Y = 0;
        }
    }
    else if(Step_count_Y != 0)
    {
        freq_count_Y += 1;
        // 在这里写你的定时任务代码
        if (freq_count_Y == 10000/temp_Y){
            GPIO_TogglePin(Motor_Y_Speed_Port,Motor_Y_Speed);
            freq_count_Y = 0;
            Step_count_Y--;
        }
    }
}
/**
 * @description: 设置X轴步进电机，转速百分比,Step为0则不设置具体步数;
 * @param {int32_t} Velocity 速度大小设置(包含正负)
 * @param {void} Step 给定具体转动步数
 * @return {*}
 */
void StepperMotor_Z_Contrl(int32_t Velocity,int32_t Step)
{
    //需要设置10us定时中断一次
    __disable_irq();  // 关闭中断
    GPIO_WriteBit(Motor_Z_DIR_Port, Motor_Z_DIR, (Velocity > 0) ? Bit_SET : Bit_RESET);
    temp_Z = (uint32_t)abs(Velocity);
    if (Step != 0){
        Step_count_Z = (int32_t)Step;
    }
    else{
        Step_count_Z = -1;
    }
    __enable_irq();   // 重新启用中断
}


/**
 * @description: X轴步进电机实时调控函数(已放入"Pid.c"中的TIM2_IRQHandler定时中断函数里面);
 * @return {*}
 */
void Stepper_Z_Task(void)
{
    if (Step_count_Z == -1)
    {
        freq_count_Z += 1;
        // 在这里写你的定时任务代码
        if (freq_count_Z == 10000/temp_Z){
            GPIO_TogglePin(Motor_Z_Speed_Port,Motor_Z_Speed);
            freq_count_Z = 0;
        }
    }
    else if(Step_count_Z != 0)
    {
        freq_count_Z += 1;
        // 在这里写你的定时任务代码
        if (freq_count_Z == 10000/temp_Z){
            GPIO_TogglePin(Motor_Z_Speed_Port,Motor_Z_Speed);
            freq_count_Z = 0;
            Step_count_Z--;
        }
    }
}
