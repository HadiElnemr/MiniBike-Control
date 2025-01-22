#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/ 

//���ʹ��
#define SERVO  TIM1->CCR4

//����ʹ��
#define PWMA1  TIM4->CCR2  //PB7
#define PWMA2  TIM4->CCR1  //PB6

//����ϵͳʹ��
#define AIN1  TIM4->CCR3  //PB8
#define AIN2  TIM4->CCR4  //PB9

//���ֵ��ʹ������
#define AEN  PAout(12)

void MiniBalance_Motor_Init(void);
void Servo_PWM_Init(uint32_t arr,uint32_t psc);
void MiniBalance_PWM_Init(uint32_t arr,uint32_t psc);
void Servo_PWM_Init(uint32_t arr,uint32_t psc)	;
#endif
