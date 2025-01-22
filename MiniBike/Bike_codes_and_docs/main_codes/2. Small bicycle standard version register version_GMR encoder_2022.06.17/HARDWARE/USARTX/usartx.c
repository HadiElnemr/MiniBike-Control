
//#include "usartx.h"
//u8 Usart2_Receive;
//int pwm_set = 0;
//u8 flag;
///**************************************************************************
//函数功能：串口3初始化
//入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
//返回  值：无
//**************************************************************************/
//void uart2_init(u32 pclk2,u32 bound)
//{  	 
//	float temp;
//	u16 mantissa;
//	u16 fraction;	   
//	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
//	mantissa=temp;				 //得到整数部分
//	fraction=(temp-mantissa)*16; //得到小数部分	 
//  mantissa<<=4;
//	mantissa+=fraction; 
//	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
//	RCC->APB1ENR|=1<<17;  //使能串口时钟 
//	GPIOA->CRL&=0XFFFF00FF; 
//	GPIOA->CRL|=0X00008B00;//IO状态设置
//	GPIOA->ODR|=1<<10;	  
//	RCC->APB1RSTR|=1<<18;   //复位串口1
//	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
//	//波特率设置
// 	USART2->BRR=mantissa; // 波特率设置	 
//	USART2->CR1|=0X200C;  //1位停止,无校验位.
//	//使能接收中断
//	USART2->CR1|=1<<8;    //PE中断使能
//	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
//	MY_NVIC_Init(0,1,USART2_IRQn,2);//组2，最低优先级 
//}

///**************************************************************************
//函数功能：串口2接收中断
//入口参数：无
//返回  值：无
//**************************************************************************/
//int USART2_IRQHandler(void)
//{	
//	if(USART2->SR&(1<<5))//接收到数据
//	{	  
//	 static	int uart_receive=0;//蓝牙接收相关变量
//	 static u8 Flag_PID,i,j,Receive[50];
//	 static float Data;
//	 uart_receive=USART2->DR; 
//	 Usart2_Receive=uart_receive;
//	
////		if(uart_receive==0x59)  Flag_sudu=2;  //低速挡（默认值）
////		if(uart_receive==0x58)  Flag_sudu=1;  //高速档   mini版不设低速档
//		
////	  if(uart_receive>10)  //默认使用app
////    {			
////		
////		
////		if(uart_receive == 0x61)//按键a，b增加减少
////		{
////				balance_point += 0.5;
//////			pwm_set += 1;
////		}
////		if(uart_receive == 0x62)
////		{
////				balance_point -= 0.5;
//////			pwm_set -= 1;
////		}
////		
//////      	if(uart_receive==0x41)  Flag_Qian=1,Flag_Hou=0;  //
//////				if(uart_receive==0x45)  Flag_Hou=1,Flag_Qian=0;  //低速挡（默认值）
//////			 if(uart_receive==0x42||uart_receive==0x43||uart_receive==0x44)	
//////				Flag_Left=0,Flag_Right=1;  //左
//////			else if(uart_receive==0x46||uart_receive==0x47||uart_receive==0x48)	    //右
//////				Flag_Left=1,Flag_Right=0;
//////			else Flag_Left=0,Flag_Right=0;//////////////刹车
////		if(uart_receive==0x43) balance_point = -1.0;
////		if(uart_receive==0x47) balance_point = 3.5;
////		if(uart_receive==0x5A)
////		{
////			balance_point = 0.5;
////		}

////  	}

//	  if(uart_receive>10)  //默认使用app
//    {			
//      	if(uart_receive==0x41)  Flag_Qian=1,Flag_Hou=0;  //
//				if(uart_receive==0x45)  Flag_Hou=1,Flag_Qian=0;  //低速挡（默认值）
//			 if(uart_receive==0x42||uart_receive==0x43||uart_receive==0x44)	
//				Flag_Left=0,Flag_Right=1,balance_point = -3.5;  //右
//			else if(uart_receive==0x46||uart_receive==0x47||uart_receive==0x48)	  
//				Flag_Left=1,Flag_Right=0,balance_point = -0.5;  //左
//			else Flag_Left=0,Flag_Right=0,balance_point=-2.0;//////////////刹车
//  	}
//		
//		if(uart_receive<10)     //备用app为：MiniBalanceV1.0  因为MiniBalanceV1.0的遥控指令为A~H 其HEX都小于10
//		{		
//      	if(uart_receive==0x01)  Flag_Qian=1,Flag_Hou=0;  //
//				if(uart_receive==0x05)  Flag_Hou=1,Flag_Qian=0;  //低速挡（默认值）			
//		  if(uart_receive==0x02||uart_receive==0x03||uart_receive==0x04)	
//			Flag_Left=0,Flag_Right=1,balance_point = -1.0;  //右
//			else if(uart_receive==0x06||uart_receive==0x07||uart_receive==0x08)	    //左
//			Flag_Left=1,Flag_Right=0,balance_point = 3.5;
//			else Flag_Left=0,Flag_Right=0,balance_point = 0.5;//////////////刹车
//  	}	

//		if(Usart2_Receive==0x7B) Flag_PID=1;  //指令起始位
//		if(Usart2_Receive==0x7D) Flag_PID=2;  //指令停止位
//		 if(Flag_PID==1)                      //记录参数
//		 {
//			Receive[i]=Usart2_Receive;
//			i++;
//		 }
//		 if(Flag_PID==2)   //执行
//		 {
//			     Show_Data_Mb=i;
//			     if(Receive[3]==0x50) 	       PID_Send=1;     //获取设备参数
//					 else  if(Receive[3]==0x57) 	 Flash_Send=1;   //掉电保存参数
//					 else  if(Receive[1]!=0x23)                    //更新PID参数
//					 {								
//						for(j=i;j>=4;j--)
//						{
//						  Data+=(Receive[j-1]-48)*pow(10,i-j);
//						}
//						switch(Receive[1])
//						 {
//							 case 0x30:  Balance_Kp=Data/1000;break;
//							 case 0x31:  Balance_Kd=Data/1000;break;
//							 case 0x32:  Velocity_Kp=Data/1000;break;
//							 case 0x33:  Velocity_Ki=Data/1000;break;
//							 case 0x34:  break;
//							 case 0x35:  break;
//							 case 0x36:  break;
//							 case 0x37:  break;
//							 case 0x38:  break;
//						 }
//					 }				 
//					 Flag_PID=0;   //相关标志位清零
//					 i=0;
//					 j=0;
//					 Data=0;
//					 memset(Receive, 0, sizeof(u8)*50);
//		 } 
//	}
//	return 0;	
//}
