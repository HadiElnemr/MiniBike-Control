#include "motor.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
	RCC->APB2ENR|=1<<2;        //PORTA时钟使能   
  RCC->APB2ENR|=1<<3;        //PORTB时钟使能   

	GPIOA->CRH&=0XFFF00FFF;   //PA11、12
	GPIOA->CRH|=0X0002B000;   //PA11复用推挽输出(用于舵机)；PA12推挽输出(使能飞轮引脚)
	
	GPIOB->CRL&=0X00FFFFFF;   
	GPIOB->CRL|=0XBB000000;   //PB6、7复用推挽输出，产生PWM(用于后轮电机驱动)
														
	GPIOB->CRH&=0XFFFFFF00;   
	GPIOB->CRH|=0X000000BB;   //PB8、9复用推挽输出，产生PWM(用于飞轮电机驱动)
}
void MiniBalance_PWM_Init(u16 arr,u16 psc)//TIM4 12  TIM
{		 					 

	MiniBalance_Motor_Init();  //初始化电机控制所需IO
	RCC->APB1ENR|=1<<2;       	//使能TIM4时钟    
	TIM4->ARR=arr;             //设定计数器自动重装值 
	TIM4->PSC=psc;             //预分频器不分频
	
	TIM4->CCMR1|=6<<4;         //CH1 PWM1模式	
	TIM4->CCMR1|=6<<12;        //CH2 PWM1模式	
	TIM4->CCMR2|=6<<4;         //CH3 PWM1模式
	TIM4->CCMR2|=6<<12;        //CH4 PWM1模式	
		
	TIM4->CCMR1|=1<<3;         //CH1预装载使能	
	TIM4->CCMR1|=1<<11;        //CH2预装载使能	 
	TIM4->CCMR2|=1<<3;         //CH3预装载使能	  
	TIM4->CCMR2|=1<<11;        //CH4预装载使能	 
	  
	TIM4->CCER|=1<<0;          //CH1输出使能	
	TIM4->CCER|=1<<4;          //CH2输出使能	   
	TIM4->CCER|=1<<8;          //CH3输出使能	   
	TIM4->CCER|=1<<12;         //CH4输出使能	
	
	TIM4->CR1=0x0080;          	 //ARPE使能 
	TIM4->CR1|=0x01;           //使能定时器4 			

	//防止飞轮转动
	AIN1 = 0;
	AIN2 = 0;
	AEN = 0;
} 

