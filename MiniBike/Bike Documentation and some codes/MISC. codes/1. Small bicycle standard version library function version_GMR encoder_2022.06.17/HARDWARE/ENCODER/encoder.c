/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：1.0
修改时间：2021-12-09

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:1.0
Update：2021-12-09

All rights reserved
***********************************************/
#include "encoder.h"
/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      
  
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;  //定时器2中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能IRQ通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;       //响应优先级3
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//使用编码器模式3
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	//Fill in each parameter in the TIM_ICInitStruct with the default value
	//把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICStructInit(&TIM_ICInitStructure);
	//Set the filter length //设置滤波器长度
	TIM_ICInitStructure.TIM_ICFilter = 10;
	//Initialize the peripheral TIMX based on the parameter TIM_ICINITSTRUCT //根据 TIM_ICInitStruct 的参数初始化外设	TIMx	
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	//Clear the update bit for Tim //清除TIM的更新标志位
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//Enable the timer to interrupt //使能定时器中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//Enable timer 2 //使能定时器2
	TIM_Cmd(TIM2, ENABLE); 
}

/**************************************************************************
函数功能：把TIM3初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      
  
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;             //TIM3中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //使能IRQ通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级 1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;          //响应优先级3       
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//使用编码器模式3
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	//Fill in each parameter in the TIM_ICInitStruct with the default value
	//把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICStructInit(&TIM_ICInitStructure);
	//Set the filter length //设置滤波器长度
	TIM_ICInitStructure.TIM_ICFilter = 10;
	//Initialize the peripheral TIMX based on the parameter TIM_ICINITSTRUCT //根据 TIM_ICInitStruct 的参数初始化外设	TIMx	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//Clear the update bit for Tim //清除TIM的更新标志位
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	//Enable the timer to interrupt //使能定时器中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//Enable timer 3 //使能定时器3
	TIM_Cmd(TIM3, ENABLE); 
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//溢出中断
	{
			    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
}

void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//溢出中断
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  	    
}
