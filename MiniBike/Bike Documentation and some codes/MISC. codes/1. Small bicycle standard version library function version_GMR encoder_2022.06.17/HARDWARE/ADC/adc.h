#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
 /**************************************************************************
作者：平衡小车之家 
淘宝店铺：http://shop114407458.taobao.com/
**************************************************************************/
#define Battery_Ch 8
#define Adjust_Ch 9
#define ZhongZhi_Ch 3
void Adc_Init(void);
u16 Get_Adc(u8 ch);
int Get_battery_volt(void); 
#endif 















