/*
 * encoder.c
 *
 *  Created on: Sep 29, 2022
 *      Author: Administrator
 */
#include "encoder.h"
/**************************************************************************
Function: Read encoder count per unit time
Input   : TIMX��Timer
Output  : none
�������ܣ���λʱ���ȡ����������
��ڲ�����TIMX����ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
   int Encoder_TIM;
   switch(TIMX)
	 {
       case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
	   case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;
	   case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;
	   default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

