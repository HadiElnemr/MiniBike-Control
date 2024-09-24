#include "motor.h"

/**************************************************************************
Function: Initialize Motor Interface
Input   : none
Output  : none
函数功能：初始化电机接口
入口参数：无
返回  值：无
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	//Enable port clock //使能端口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//Port configuration //端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	//Push-pull output //推挽输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M
	//Initialize GPIO with the specified parameters   //根据设定参数初始化GPIO
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//IO输出0，防止电机乱转
  GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

/**************************************************************************
Function: Initialize PWM to drive motor
Input   : arr：Auto reload value；psc：Prescaler coefficient
Output  : none
函数功能：初始化PWM，用于驱动电机 
入口参数：arr：自动重装值；psc：预分频系数
返回  值：无
**************************************************************************/
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	

	MiniBalance_Motor_Init();														 //飞轮引脚使能引脚初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //开启定时器4时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启GPIOB时钟
	
	//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr;              //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //设置时钟分割
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);       //初始化定时器
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH1使能预装载寄存器
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH2使能预装载寄存器
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH3使能预装载寄存器
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH4使能预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);                //使能TIM4在ARR上的预装载寄存器
	
	TIM_Cmd(TIM4,ENABLE);                              //使能定时器4
} 

/**************************************************************************
Function: Initialize PWM to drive motor
Input   : arr：Auto reload value；psc：Prescaler coefficient
Output  : none
函数功能：初始化PWM，用于驱动舵机 
入口参数：arr：自动重装值；psc：预分频系数
返回  值：无
**************************************************************************/
void Servo_PWM_Init(u16 arr,u16 psc)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//定时器基本配置
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

	//设置PWM输出模式
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 

	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  
	TIM_CtrlPWMOutputs(TIM1,ENABLE);			//TIM1必须要这句话才能输出PWM

	TIM_ARRPreloadConfig(TIM1, ENABLE);   //使能TIMx在ARR上的预装载寄存器	
	TIM_Cmd(TIM1, ENABLE);  							//使能TIM1
}

