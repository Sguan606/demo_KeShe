/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-21 00:54:18
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-05-21 02:28:13
 * @FilePath: \demo_KeShe\Hardware\Pid.c
 * @Description: [标准库]实现PID调控的函数实现;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "Pid.h"

//对温度PID计算的积分限幅
#define TempADC_S_Max 100
#define TempADC_S_Min -100 
extern float target_temp;

/**
 * @description: PID闭环控制器，参数定义列表（待调）
 * @return {*}
 */
float Temp_Kp=0.2f,Temp_Ki=0.001f;  //Ki为Kp的俩百分之一
// float Temp_Kd=0.0001f;   //可能会用到PID里面的D项
int Temp_Conteol_Stop = 0;  //PID调控积分清零，让PID调控更可控


/**
 * @description: 用于进行PID各项的计算，得到real_temp->target_temp的调控力度;
 * @param {int} target_temp         输入温度控制的目标数值
 * @param {int} real_temp           实际测量得到的真实数值
 * @return {*}
 */
float Temp_Calculate(float target_temp,float real_temp)
{
    static float Temp_Err,TempADC_S;
    static float Err_Wave=0.7;
    float Err0,Err1,temp;
    //1.计算偏差
    Err0=real_temp - target_temp;

    //2.低通滤波
    Err1=(1-Err_Wave)*Err0+Err_Wave*Temp_Err;
    Temp_Err=Err1;
    //3.积分
    TempADC_S += Temp_Err;

    //4.积分限幅Limit
    TempADC_S = TempADC_S>TempADC_S_Max?TempADC_S_Max:(TempADC_S<(TempADC_S_Min)?(TempADC_S_Min):TempADC_S);
    if(Temp_Conteol_Stop==1)TempADC_S=0,Temp_Conteol_Stop=0;
    //5.速度环核心
    temp=Temp_Kp*Temp_Err + Temp_Ki*TempADC_S;

    return temp;
}




/**
 * @description: PID调控（设定定时器每10us调控一次）
 * @return {*}
 */
void Pid_Control_Task(void)
{
    float Temp_Out;
    //1.传感器测得真实值(ADC采样真实温度real_temp)
    float real_temp = Get_RealTemp();

    //2.数据导入到PID控制器中，并导出输出结果
    Temp_Out = Temp_Calculate(target_temp,real_temp);

    //3.实际控制
    Temp_Set(target_temp + Temp_Out);
}



/**
  * 函    数：TIM2中断函数（每10us定时中断一次）
  * (如果需要修改定时器中断的时间间隔，请前往"Timer.c"中调整TIM定时器的初始化预分频系数and分频CRR)
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
        Pid_Control_Task();

        Stepper_X_Task();
        Stepper_Y_Task();
        Stepper_Z_Task();

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}
