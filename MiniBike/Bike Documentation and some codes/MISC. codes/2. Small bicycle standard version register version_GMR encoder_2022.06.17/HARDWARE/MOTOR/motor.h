#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

//后轮使用  Rear wheel use
#define PWMA1  TIM4->CCR2  //PB7
#define PWMA2  TIM4->CCR1  //PB6

//飞轮系统使用 Flywheel (->Frontwheel) system use
#define AIN1  TIM4->CCR3  //PB8
#define AIN2  TIM4->CCR4  //PB9

//飞轮电机使能引脚  Flywheel motor enable pin
#define AEN  PAout(12)


void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
