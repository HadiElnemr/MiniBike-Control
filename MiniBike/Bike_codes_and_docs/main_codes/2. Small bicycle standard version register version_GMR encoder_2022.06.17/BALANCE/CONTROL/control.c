#include "control.h"
#include "math.h"
/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8 COUNT;

int Voltage_Temp,Voltage_Count,Voltage_All;  //计算电压相关变量
float BalancePoint_Offset = 0; 							 //电位器偏差，用于调节机械中值
float LeftControl  = 3.0f;  //左转向的幅度
float RightControl = 3.0f;	//右转向的幅度
float delta=0, delta_dot=0, last_delta_dot=0, last_delta_dot_2=0; //δ[i],δ'[i],δ'[i-1],δ'[i-2].
float k1=-92.2973, k2=-8.6746, k3=10.5355;	//LQR反馈系数
int ki=4, kp=16, kd=4;	//PID参数（放大100倍后，整数易于调试）
int u; //放大10000后的输入	
/**************************************************************************
函数功能：所有的控制代码都在这里面
					5ms定时中断由MPU6050的INT引脚触发
					严格保证采样和数据处理的时间同步
**************************************************************************/
int EXTI15_10_IRQHandler(void) 
{
	if(INT==0)
		{
			EXTI->PR=1<<12;		//清除LINE12上的中断标志位
			if(delay_flag==1)
				{
					if(++delay_50==10)
						delay_50=0, delay_flag=0;		        //给主函数提供精准延时
				}
			if(COUNT<3)
				{
					COUNT += 1;														//时间变量计数，用于延迟
					Get_Angle(Way_Angle);                 //更新姿态					
					Voltage_Temp=Get_battery_volt();		  //读取电池电压		
					Voltage_Count++;                      //平均值计数器
					Voltage_All+=Voltage_Temp;            //多次采样累积
					if(Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//求平均值		
					Led_Flash(100);   												//常规模式蓝灯慢闪
					SERVO_BIAS=(Get_Adc(Adjust_Ch)-1024)/12;	//舵机中值调节
					
					if(Flag_Stop==1) //在电机关闭的状态才可以调节中值，避免平衡过程中数值跳动
					{
						BalancePoint_Offset = (5.06f/1024.0f)*(Get_Adc(ZhongZhi_Ch)-1024); //可调节范围：±5.0° ， 由式中5.06f数值决定
						//去除第二位以后的小数点
						BalancePoint_Offset = (int)(BalancePoint_Offset*10);
						BalancePoint_Offset = (int)(BalancePoint_Offset/10) + ((int)BalancePoint_Offset%10)/10.0f;	
						balance_point = BalancePoint_Offset + balance_point_diff;//调节中值，使用偏差的形式，使得中值不会被电位器锁死
					}
					return 0;
				}                                                             
			COUNT=0;																											 //20ms控制一次
			Encoder=-Read_Encoder(2)/38.4615f;                             //读取编码器的值（使用500线GMR编码器）
			Get_Angle(Way_Angle);                                          //更新姿态
			phi_dot = Gyro_Balance/16.4f;																	 //更新角速度
			delta = (Servo - MID)/3.65f;																	 //更新前叉转角
			Key();                                                         //扫描按键状态 单击双击可以改变小车运行状态	 
			if(Put_Down(phi,Encoder)==1)  Flag_Stop=0;               			 //姿态识别启动
      if(Turn_Off(phi,Voltage)==0&&Flag_Stop==0)               			 //如果不存在异常
				{
					Get_RC();    																							 //接收app控制
					delta_dot = -(k1*(phi-balance_point)+k2*phi_dot+k3*delta); //LQR控制
					delta_dot *= 100;																					 //放大100倍.
					delta_dot = (int)delta_dot; 															 //取整,相当于精确到小数点后两位.
					u = ki*delta_dot + kp*(delta_dot-last_delta_dot) + kd*(delta_dot-2*last_delta_dot+last_delta_dot_2); //PID控制
					Servo += u/10000;																					 //输出到舵机
					if( Servo < (MID-130) ) Servo = MID - 130;                 //舵机角度限制
					if( Servo > (MID+130) ) Servo = MID + 130;	 					     //舵机角度限制
					last_delta_dot_2 = last_delta_dot;											   //保存本次的变量
					last_delta_dot = delta_dot;
					Motor=Incremental_PI_A(Encoder,-60);                  		 //后轮电机控制
					Pwm_Limit();																							 //PWM限幅
					Set_Pwm(-Motor);																					 //赋值给PWM寄存器
				}
				else Set_Pwm(0);																						 //按键按下或者小车倒下关停电机
			
		}
	
		return 0;

}

/**************************************************************************
函数功能：接受APP控制
入口参数：无
返回  值：无
**************************************************************************/
void Get_RC(void)
{
	static u8 TurnFlag=0;
	static float turn;
  static u8 Last_L_Flag,Last_R_Flag;
	static u8 updateTime;
	static u8 WaitTime;
	
	//发生转向突变
	if((Last_L_Flag&&Flag_Right)||(Last_R_Flag&&Flag_Left))
	{
		//开始计时，1秒后释放
		if(++WaitTime<50) return;
		else
			WaitTime = 60; //已经允许控制，锁住变量防止循环
	}
	else
		WaitTime = 0;
	
	
	//接收到app控制信号，读取1次机械中值
	if((Flag_Right||Flag_Left)&&TurnFlag==0)
	{
		turn = balance_point;
		TurnFlag = 1;
	}
	
	//左右控制
	if(Flag_Left) 				balance_point = turn + LeftControl;
	else if (Flag_Right)  balance_point = turn - RightControl;

	//APP无控制，机械中值复位
	if(Flag_Right==0&&Flag_Left==0&&TurnFlag==1)
	{
		balance_point = turn;
		TurnFlag = 0;
	}
	

	//1秒更新一次保存的标志位，用于检查方向突变
	if(++updateTime>50)
	{
		//保存本次标志位
		Last_L_Flag = Flag_Left;
		Last_R_Flag = Flag_Right;
		updateTime = 0;
	}

}

/**************************************************************************
函数功能：舵机角度控制
入口参数：舵机角度
返回  值：舵机控制PWM
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Pwm, Bias, Last_bias, Last_bias_2;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=50*(Bias-Last_bias)+5*Bias+60*(Bias-2*Last_bias+Last_bias_2);   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias_2=Last_bias;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1)
{
		if(moto1>0)			PWMA1=7200,			PWMA2=7200-moto1;
		else 	          PWMA2=7200,			PWMA1=7200+moto1;

		SERVO=Servo + SERVO_BIAS;	//舵机控制
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Pwm_Limit(void)
{	
	  int Amplitude=6900;    //===PWM满幅是7200 限制在6900
    if(Motor<-Amplitude) Motor=-Amplitude;	
		if(Motor>Amplitude)  Motor=Amplitude;		
}
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	static u8 flag=0;
	tmp=click_N_Double(50); 
	if(tmp==1)
	{
		if(flag==0)
		{
			if(Flag_Stop==1&&(phi>=-10&&phi<=10))  Flag_Stop=0;
		}
		else
			Flag_Stop=1;
		
		if(Flag_Stop==1) flag=0;
		else flag=1;
	}
	if(tmp==2) Flag_Show=!Flag_Show;//双击控制小车的显示状态
}
/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off(float angle, int voltage)
{
		u8 temp;
		if(angle<-40||angle>40||1==Flag_Stop||voltage<710||Roll<-20)//电池电压低于6.1V关闭电机
		{	                                                 //===倾角大于40度关闭电机
			temp=1;                                            //===Flag_Stop置1关闭电机
			SERVO=MID + SERVO_BIAS;
			Flag_Stop=1;	
		}
		else
		temp=0;
		return temp;			
}
	
/**************************************************************************
函数功能：获取角度
入口参数：获取角度的算法 1：DMP  2：卡尔曼滤波 3：互补滤波
返回  值：无
**************************************************************************/
void Get_Angle(u8 way)
{ 
		if(way==1)                           //===DMP的读取在数据采集中断提醒的时候，严格遵循时序要求
		{	
				Read_DMP();                      //===读取加速度、角速度、倾角
				phi=Pitch;             //===更新平衡倾角
				Gyro_Balance=gyro[1];  //===更新平衡角速度
		}			
}
/**************************************************************************
函数功能：检测小车是否被放下
入口参数：(外)倾角,编码器值
返回  值：布尔值
**************************************************************************/
int Put_Down(float angle,int encoder)
{
	static u8 flag,count;
	if(flag==0)
		{
			//条件1,小车是在平衡点附近.
			if(angle>(balance_point-10) && angle<(balance_point+10) && encoder<2 && encoder>-2 && Flag_Stop==1) flag=1;
		}
	if(flag==1)
		{
			//超时不再等待500ms.
			if(++count>50)
				{
					count=0;
					flag=0;
				}
			//条件2,小车的轮胎在未上电的时候被人为转动.
			if(encoder<-8)
				{
					flag=0;
					return 1;
				}
		}
	return 0;
}

