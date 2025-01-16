#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8 Way_Angle=1;                             //获取角度的算法，1：四元数  Algorithm for obtaining angles, 1: Quaternions
u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //蓝牙遥控相关的变量 Bluetooth remote control related variables
u8 Flag_Stop=1,Flag_Show=0;      						//停止标志位和 显示标志位 默认停止 显示打开 Stop Flag Bit and Display Flag Bit Default Stop Display On
int Encoder;             										//左右编码器的脉冲计数 Pulse counting for left and right encoders
int Motor;                           				//电机PWM变量 Motor PWM Variables
int Servo=MID;															//舵机PWM变量 Servo PWM Variables
int Voltage;                                //电池电压采样相关的变量 Variables related to battery voltage sampling
float phi,phi_dot,Gyro_Balance; 						//倾角 倾角角速度 平衡陀螺仪 Tilt Angle, Tilt Angular Velocity, Balance Gyroscope
float Show_Data_Mb;                         //全局显示变量，用于显示需要查看的数据 Global display variable to display the data to be viewed
u8 delay_50,delay_flag,PID_Send,Flash_Send;	//延时和调参等变量 Variables such as delay and modulation
float balance_point = 0.0f;									//机械中值 mechanical median
float balance_point_diff;           			  //机械中值调整位 Mechanical median adjustment position
int SERVO_BIAS = 0; 												//舵机角度零点调整 Rudder Angle Zero Adjustment

int main(void)
{
	Stm32_Clock_Init(9);            //系统时钟设置							System Clock Setting
	delay_init(72);                 //延时初始化					    	Delay initialisation
	JTAG_Set(JTAG_SWD_DISABLE);			//关闭JTAG接口才可以使用显示屏	  	  Close the JTAG interface to use the display.
	JTAG_Set(SWD_ENABLE);           //打开SWD接口 可以利用主板的SWD接口调试    Switch on the SWD interface to use the SWD interface on the motherboard for debugging.
	LED_Init();                     //初始化与 LED 连接的硬件接口				Initialise the hardware interface to the LEDs
	KEY_Init();                     //按键初始化							Key initialisation
	OLED_Init();                    //OLED初始化							Initialise OLED
	uart_init(72,128000);           //初始化串口1							Initialise serial port 1
	uart3_init(36,9600);            //串口3初始化							Initialise serial port 3
	MiniBalance_PWM_Init(7199,0);   //初始化PWM 10KHZ 						Initialise PWM 10KHZ
	Encoder_Init_TIM2();            //编码器接口							Encoder Interface
	Adc_Init();                     //adc初始化								Initialise adc
	IIC_Init();                     //模拟IIC初始化							Analogue IIC initialisation
	MPU6050_initialize();           //MPU6050初始化							MPU6050 initialisation
	DMP_Init();                     //初始化DMP								Initialise DMP
	Servo_PWM_Init(9999,144);   		//初始化舵机PWM（50Hz）				 Initialise servo PWM (50Hz)
 	EXTI_Init();                    //启用外部中断(MPU6050提供5ms外部中断)     Enable external interrupt (MPU6050 provides 5ms external interrupt)

	while(1)
	{
			if(Flag_Show==0)		//使用MiniBalance APP和OLED显示屏	     Using MiniBalance APP and OLED display
			{
				APP_Show();				//打印数据到APP 					Print data to APP
				oled_show();			//OLED显示数据						OLED displays data
			}
			else								//使用MiniBalance上位机 上位机使用的时候需要严格的时序，故此时关闭app监控部分和OLED显示屏
			{									// Using the MiniBalance Uplink Strict timing is required when using the Uplink,
												// so the app monitoring section and OLED display are turned off at this time.
				DataScope();			//开启MiniBalance上位机      Switching on the MiniBalance Uplink
			}
			delay_flag=1;
			delay_50=0;
			while(delay_flag);	//通过MPU6050的INT中断实现的50ms精准延时       50ms precision delay via INT interrupt of MPU6050

	}
}
