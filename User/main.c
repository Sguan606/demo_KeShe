/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-21 00:53:13
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-05-21 15:47:59
 * @FilePath: \demo_KeShe\User\main.c
 * @Description: [标准库]实现课设要求的主函数入口;
 * 1.对于步进电机：
 * 我这里使用的是自己经常用的A4988模块，使用stm32输出电平脉冲到STEP引脚，控制角度和角速度，DIR引脚控制转动方向;
 * 2.对于温控系统：
 * 我不太了解温度采集是怎么实现的(需要参考具体实物)，
 * 这里就暂时假设使用ADC单通道采样得到0-4095的温度数值，然后与target_temp做差值，差值就是我们PID要调控的地方;
 * 3.对于PID闭环控制算法：
 * 比较简单的Kp比例项，Ki积分项，Kd微分项运算
 * 4.对于TIM定时器：
 * 我们设置的是每10us定时中断，去调控步进电机转速，同时使用PID函数修正温度差值。
 * 
 * 注释ps：唯一没有写好的是"Temp.c"中针对实际温控设备控制的函数；
 * 
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "AD.h"
#include "Timer.h"

#include "Stepper_Motor.h"
#include "Temp.h"
#include "Pid.h"

/**
 * @description: 通过修改全局变量target_temp的值，控制输出的温度大小;
 * @return {*}
 */
float target_temp = 20.0f;


int main(void)
{
	/*模块初始化*/
	Timer_Init();		//定时中断初始化
	AD_Init();			//初始化ADC采样
	Stepper_Init();
	
	/*函数功能的调用*/
	//对于42步进电机，200步一圈，因为步进电机一个上升沿脉冲触发一步，输入400参数就走一圈;
	StepperMotor_X_Contrl(30,800);		//速度30%，转动2圈
	// StepperMotor_X_Contrl(72,400);		//速度72%，转动1圈
	// StepperMotor_X_Contrl(9,0);			//速度9%，一直旋转
	//对于温度调控（直接修改target_temp的数值）
	target_temp = 52.01314f;
	while (1)
	{
		//while循环中运行长期执行的函数
		Delay_ms(10);
	}
}

