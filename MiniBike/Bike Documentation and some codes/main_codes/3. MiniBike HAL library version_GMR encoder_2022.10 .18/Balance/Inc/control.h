#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265

#define DIFFERENCE 160
#define MID 750
void EXTI15_10_IRQHandler(void);
int balance(float angle,float gyro);
int servo(int servo_input);
int turn(int encoder_left,int encoder_right,float gyro);
void Set_Pwm(int moto1);
void Key(void);
void Pwm_Limit(void);
u8 Turn_Off(float angle, int voltage);
void Get_Angle(uint8_t way);
int myabs(int a);
int Put_Down(float Angle,int encoder);
int Incremental_PI_A (int Encoder,int Target);
void Get_RC(void);
extern float Balance_Pwm,Servo_Pwm,Turn_Pwm;
extern int SERVO_BIAS;
extern u8 flag;
#endif
