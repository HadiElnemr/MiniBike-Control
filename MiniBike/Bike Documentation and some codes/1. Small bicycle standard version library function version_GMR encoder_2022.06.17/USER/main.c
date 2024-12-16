#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8 Way_Angle=1;                             //获取角度的算法，1：四元数  //Get the angle algorithm, 1: quaternion
u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //蓝牙遥控相关的变量	//Bluetooth remote control related variables
u8 Flag_Stop=1,Flag_Show=0;      						//停止标志位和 显示标志位 默认停止 显示打开 //Stop flag and display flag default stop display open
int Encoder;             										//左右编码器的脉冲计数 //Pulse count of left and right encoder
int Motor;                           				//电机PWM变量 //Motor PWM variable
int Servo=MID;															//舵机PWM变量 //Servo PWM variable
int Voltage;                                //电池电压采样相关的变量 //Battery voltage sampling related variables
float phi,phi_dot,Gyro_Balance; 						//倾角 倾角角速度 平衡陀螺仪 //Inclination angle inclination angular velocity balance gyroscope
float Show_Data_Mb;                         //全局显示变量，用于显示需要查看的数据 //Global display variable for displaying data that needs to be viewed
u8 delay_50,delay_flag,PID_Send,Flash_Send;	//延时和调参等变量 //Delay and parameter adjustment variables
float balance_point = 0.0f;									//机械中值 //Mechanical center
float balance_point_diff;           			  //机械中值调整位 //Mechanical center adjustment bit
int SERVO_BIAS = 0; 												//舵机角度零点调整 	//Servo angle zero point adjustment

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组2 //Interrupt priority group 2
	delay_init();
	JTAG_Set(JTAG_SWD_DISABLE);			//关闭JTAG接口才可以使用显示屏 //Close the JTAG interface to use the display
	JTAG_Set(SWD_ENABLE);           //打开SWD接口 可以利用主板的SWD接口调试 //Open the SWD interface to debug using the SWD interface of the motherboard
	LED_Init();                     //初始化与 LED 连接的硬件接口 //Initialize the hardware interface connected to the LED
	KEY_Init();                     //按键初始化 //Key initialization
	OLED_Init();                    //OLED初始化 //OLED initialization
	uart_init(128000);              //初始化串口1 128000bps //Initialize serial port 1 128000bps
	uart3_init(9600);               //串口3初始化 9600bps //Serial port 3 initialization 9600bps
	MiniBalance_PWM_Init(7199,0);   //初始化PWM 10KHZ //Initialize PWM 10KHZ
	Encoder_Init_TIM2();            //编码器接口 //Encoder interface
	Adc_Init();                     //adc初始化 //adc initialization
	IIC_Init();                     //模拟IIC初始化 //Analog IIC initialization
	MPU6050_initialize();           //MPU6050初始化 //MPU6050 initialization
	DMP_Init();                     //初始化DMP //Initialize DMP
	Servo_PWM_Init(9999,144);   		//初始化舵机PWM（50Hz //Initialize servo PWM (50Hz)
 	MiniBalance_EXTI_Init();        //启用外部中断(MPU6050提供5ms外部中断) //Enable external interrupt (MPU6050 provides 5ms external interrupt)

	while(1)
	{
			if(Flag_Show==0)		//使用MiniBalance APP和OLED显示屏 //Use MiniBalance APP and OLED display
			{
				APP_Show();				//打印数据到APP //Print data to APP
				oled_show();			//OLED显示数据 //OLED display data
			}
			else								//使用MiniBalance上位机 上位机使用的时候需要严格的时序，故此时关闭app监控部分和OLED显示屏 //Use MiniBalance host computer. When using the host computer, strict timing is required, so close the app monitoring part and OLED display
			{
				DataScope();			//开启MiniBalance上位机 //Open MiniBalance host computer
			}
			delay_flag=1;
			delay_50=0;
			while(delay_flag);	//通过MPU6050的INT中断实现的50ms精准延时 //50ms precise delay implemented through MPU6050 INT interrupt

	}
}
