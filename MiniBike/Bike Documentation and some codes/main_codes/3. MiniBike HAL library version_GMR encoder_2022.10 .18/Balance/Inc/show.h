#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
void oled_show(void);
void APP_Show(void);
void DataScope(void);
extern short test_num;

// extern float SERVO;
extern float LeftControl;
extern float RightControl;
// extern float kp,ki,kd;
extern float balance_point;
extern int Voltage;
extern int Encoder;
extern int Motor;
extern int Servo;
// extern float phi;
extern u8 Flag_Stop;
extern u8 Flag_Show;
extern u8 PID_Send;

#endif
