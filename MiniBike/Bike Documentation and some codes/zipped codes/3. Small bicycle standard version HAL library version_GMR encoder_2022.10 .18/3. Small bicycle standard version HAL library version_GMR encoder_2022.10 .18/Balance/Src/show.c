#include "show.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
float Vol;
extern float Angle_Turn;
extern float delta;
extern int SERVO_BIAS;
extern float phi_dot;
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void oled_show(void)
{

		//=============��һ����ʾС��ģʽ=======================//
		OLED_ShowString(0,0,"Motor:");
		if(Flag_Stop) OLED_ShowString(70,0,"OFF");
		else					OLED_ShowString(70,0," ON");
		//=============������=======================//
		                      OLED_ShowString(00,10,"Encoder");
		if( Encoder<0)				OLED_ShowString(80,10,"-"),
		                      OLED_ShowNumber(95,10,-Encoder,3,12);
		else                 	OLED_ShowString(80,10,"+"),
		                      OLED_ShowNumber(95,10, Encoder,3,12);

  	//=============������=======================//
		                      OLED_ShowString(00,20,"SERVO");
		if(Servo<0)		 			  OLED_ShowString(80,20,"-"),
		                      OLED_ShowNumber(95,20,-(Servo+SERVO_BIAS),3,12);
		else               		OLED_ShowString(80,20,"+"),
		                      OLED_ShowNumber(95,20,(Servo+SERVO_BIAS),3,12);
		//=============��������ʾ��ѹ=======================//
		                      OLED_ShowString(00,30,"Volta");
		                      OLED_ShowString(58,30,".");
		                      OLED_ShowString(80,30,"V");
		                      OLED_ShowNumber(45,30,Voltage/100,2,12);
		                      OLED_ShowNumber(68,30,Voltage%100,2,12);
		 if(Voltage%100<10) 	OLED_ShowNumber(62,30,0,2,12);
		//=============��������ʾ�Ƕ�=======================//
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


		//=============ˢ��=======================//
		OLED_Refresh_Gram();
	}
/**************************************************************************
�������ܣ���APP��������
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void APP_Show(void)
{
		static u8 flag;
	  int app_2,app_3,app_4;
		app_4=(Voltage-710)*2/3;		if(app_4<0)app_4=0;if(app_4>100)app_4=100;   //�Ե�ѹ���ݽ��д���
		app_3=Encoder/1.3; if(app_3<0)app_3=-app_3;			                   			 //�Ա��������ݾ������ݴ�������ͼ�λ�
		app_2=Encoder/1.3;  if(app_2<0)app_2=-app_2;
	  flag=!flag;
	if(PID_Send==1)//����PID����
	{
		//��ӡkp��ki��kd�Լ���е��ֵ��app���ν���
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d$",(int)(kp),(int)(ki),(int)(kd),(int)(balance_point*100),(int)(LeftControl*10),(int)(RightControl*10),0,0);//��ӡ��APP����
		PID_Send=0;
	}
   else	if(flag==0)// ��ʾ���������������Ƕ�����ҳ
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)phi); //��ӡ��APP����

	 else //������ʾ�Ƕȡ����ٶȡ����PWM
		 printf("{B%d:%d:%lu}$",(int)phi,(int)phi_dot,(unsigned long)SERVO);//��ӡ��APP���� ��ʾ����
}
/**************************************************************************
�������ܣ�����ʾ��������λ���������� �ر���ʾ��
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void DataScope(void)
{
    Vol=(float)Voltage/100;
		DataScope_Get_Channel_Data( phi, 1 );       //��ʾ�Ƕ� ��λ���ȣ��㣩
		DataScope_Get_Channel_Data( phi_dot, 2 ); //��ʾ���ٶ�
		DataScope_Get_Channel_Data( SERVO, 3 );       //��ʾ�����pwmֵ
//		DataScope_Get_Channel_Data( 0 , 4 );
//		DataScope_Get_Channel_Data(0, 5 ); //����Ҫ��ʾ�������滻0������
//		DataScope_Get_Channel_Data(0 , 6 );//����Ҫ��ʾ�������滻0������
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
