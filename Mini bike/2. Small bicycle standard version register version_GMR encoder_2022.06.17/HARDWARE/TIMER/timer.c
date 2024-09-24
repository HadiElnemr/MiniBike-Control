#include "timer.h"
/**************************************************************************
函数功能：定时器3通道3输入捕获初始化
入口参数：入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
**************************************************************************/
void Servo_PWM_Init(u16 arr,u16 psc)	//pa11  tim1 ch4
{	 
	
	RCC->APB2ENR|=1<<11;       //使能TIM1时钟
	RCC->APB2ENR|=1<<2;    		 //使能PORTA时钟  
	GPIOA->CRH&=0XFFFF0FFF;    //PORTA11复用输出
	GPIOA->CRH|=0X0000B000;    //PORTA11复用输出
	
	TIM1->ARR=arr;             //设定计数器自动重装值 
	TIM1->PSC=psc;             //预分频器不分频
	TIM1->CCMR2|=6<<12;        //CH4 PWM1模式	
	TIM1->CCMR2|=1<<11;        //CH4预装载使能	
	TIM1->CCER|=1<<12;         //CH4输出使能
	TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
	TIM1->CR1=0x0080;          //ARPE使能 
	TIM1->CR1|=0x01;           //使能定时器1
	TIM1->CCR1=750;	
	
}
