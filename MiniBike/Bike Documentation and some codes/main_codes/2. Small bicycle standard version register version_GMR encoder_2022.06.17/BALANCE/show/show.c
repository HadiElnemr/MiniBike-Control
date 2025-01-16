#include "show.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
extern float Angle_Turn;
extern float delta;
extern int SERVO_BIAS;
extern float phi_dot;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{
	
		//=============第一行显示小车模式=======================//	
		OLED_ShowString(0,0,"Motor:");
		if(Flag_Stop) OLED_ShowString(70,0,"OFF");
		else					OLED_ShowString(70,0," ON");
		//=============第三行=======================//	
		                      OLED_ShowString(00,10,"Encoder");
		if( Encoder<0)				OLED_ShowString(80,10,"-"),
		                      OLED_ShowNumber(95,10,-Encoder,3,12);
		else                 	OLED_ShowString(80,10,"+"),
		                      OLED_ShowNumber(95,10, Encoder,3,12);
		
  	//=============第四行=======================//		
		                      OLED_ShowString(00,20,"SERVO");
		if(Servo<0)		 			  OLED_ShowString(80,20,"-"),
		                      OLED_ShowNumber(95,20,-(Servo+SERVO_BIAS),3,12);
		else               		OLED_ShowString(80,20,"+"),
		                      OLED_ShowNumber(95,20,(Servo+SERVO_BIAS),3,12);	
		//=============第五行显示电压=======================//
		                      OLED_ShowString(00,30,"Volta");
		                      OLED_ShowString(58,30,".");
		                      OLED_ShowString(80,30,"V");
		                      OLED_ShowNumber(45,30,Voltage/100,2,12);
		                      OLED_ShowNumber(68,30,Voltage%100,2,12);
		 if(Voltage%100<10) 	OLED_ShowNumber(62,30,0,2,12);
		//=============第六行显示角度=======================//
		                      OLED_ShowString(0,40,"Angle");
		if(phi<0)		OLED_ShowNumber(45,40,phi+360,3,12);
		else					        OLED_ShowNumber(45,40,phi,3,12);
		
		
		OLED_ShowString(0,50,"Zhongzhi");
		if(balance_point<0)
		{
			OLED_ShowString(70,50,"-");
			OLED_ShowNumber(80,50,-balance_point,2,12);
			OLED_ShowString(96,50,".");
			OLED_ShowNumber(106,50,(-balance_point-(int)(-balance_point))*10,1,12);
		}	
		else
		{
			OLED_ShowString(70,50,"+");
			OLED_ShowNumber(80,50,balance_point,2,12);
			OLED_ShowString(96,50,".");
			OLED_ShowNumber(106,50,(balance_point-(int)(balance_point))*10,1,12);
		}			
	
		
		//=============刷新=======================//
		OLED_Refresh_Gram();	
	}
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void APP_Show(void)
{    
		static u8 flag;
	  int app_2,app_3,app_4;
		app_4=(Voltage-710)*2/3;		if(app_4<0)app_4=0;if(app_4>100)app_4=100;   //对电压数据进行处理
		app_3=Encoder/1.3; if(app_3<0)app_3=-app_3;			                   			 //对编码器数据就行数据处理便于图形化
		app_2=Encoder/1.3;  if(app_2<0)app_2=-app_2;
	  flag=!flag;
	if(PID_Send==1)//发送PID参数
	{
		//打印kp、ki、kd以及机械中值到app调参界面
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d$",(int)(kp),(int)(ki),(int)(kd),(int)(balance_point*100),(int)(LeftControl*10),(int)(RightControl*10),0,0);//打印到APP上面	
		PID_Send=0;	
	}	
   else	if(flag==0)// 显示电量、编码器、角度在首页
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)phi); //打印到APP上面
	 
	 else //波形显示角度、角速度、舵机PWM
		 printf("{B%d:%d:%d}$",(int)phi,(int)phi_dot,SERVO);//打印到APP上面 显示波形
}
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void DataScope(void)
{   
    Vol=(float)Voltage/100;
		DataScope_Get_Channel_Data( phi, 1 );       //显示角度 单位：度（°）
		DataScope_Get_Channel_Data( phi_dot, 2 ); //显示角速度
		DataScope_Get_Channel_Data( SERVO, 3 );       //显示舵机的pwm值
//		DataScope_Get_Channel_Data( 0 , 4 );   
//		DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0, 7 );
//		DataScope_Get_Channel_Data( 0, 8 ); 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(3);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}
