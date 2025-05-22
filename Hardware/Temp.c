/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-21 00:54:18
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-05-21 02:14:42
 * @FilePath: \demo_KeShe\Hardware\Temp.c
 * @Description: [标准库]实现ADC单通道采样，获取real_temp;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Temp.h"

#define Temp_Max 100.0f
#define Temp_Min 0.0f


/**
 * @description: 默认使用PA0引脚进行ADC温度采样;
 * 注释ps：修改采样引脚，请前往"AD.c"文件中，修改对应的ADC初始化;
 * @return {*}
 */
float Get_RealTemp(void)
{
    float ADValue,real_temp;
    float Temp_Scope = (float)(Temp_Max - Temp_Min);
    ADValue = AD_GetValue();					//获取AD转换的值
    real_temp = (float)ADValue / 4095 * Temp_Scope + Temp_Min;		//将AD值线性变换到0~Temp的范围，表示温度范围
    return real_temp;
}


/**
 * @description: 实现你们自己的温度开环控制代码（后面在我实现的Pid.c中会调用到）
 * @param {float} temp
 * @return {*}
 */
void Temp_Set(float temp)
{
    //在这个里面实现针对你们自己设备的温度控制代码；
    //不同设备，代码不同，我这里不能写出
}

