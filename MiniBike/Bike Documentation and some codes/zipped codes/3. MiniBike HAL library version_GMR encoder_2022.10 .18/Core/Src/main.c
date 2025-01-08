/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
u8 Way_Angle=1;                             //��ȡ�Ƕȵ��㷨��1����Ԫ��
u8 Flag_front,Flag_back,Flag_Left,Flag_Right,Flag_velocity; //����ң����صı���
u8 Flag_Stop=1,Flag_Show=0;      			//ֹͣ��־λ����ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder;             					//���������������
int Motor;                           		//���PWM����
int Servo=MID;								//���PWM����
int Voltage;                                //��ص�ѹ������صı���
float phi,phi_dot,Gyro_Balance; 			//��� ��ǽ��ٶ� ƽ��������
float Show_Data_Mb;                         //ȫ����ʾ������������ʾ��Ҫ�鿴������
u8 PID_Send;
u8 delay_50,delay_flag,Flash_Send;	//��ʱ�͵��εȱ���
float balance_point = 0.0f;					//��е��ֵ
float balance_point_diff;           		//��е��ֵ����λ
int SERVO_BIAS = 0; 						//����Ƕ�������
float Balance_Kp=22500,Balance_Kd=108,Velocity_Kp=16000,Velocity_Ki=80,Turn_Kp=4200,Turn_Kd=0;//PID�������Ŵ�100����
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); //�ȹر����õ��ⲿ�жϣ���ֹ��ʼ�������н����ж�
  delay_init(); 						//�ͷ�SysTick��ʹ��DWT��ΪHAL��ʱ��

  //HAL�ⲻ���Զ����ò���
  HAL_ADCEx_Calibration_Start(&hadc1);//����ADC1У׼����У׼������ADC������׼ȷ

  /* PWM������Ƶ�� */
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);//������ʱ��4ͨ��1��2��PWM��������Ƶ��

  /* PWM������ƶ�� */
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);//������ʱ��1ͨ��4��PWM��������ƶ��

  /* ʹ�ܱ����� */
  HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);//������ʱ��2�ı�����ģʽ
  HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);//������ʱ��3�ı�����ģʽ

  OLED_Init();  						//OLED��ʼ��
  MPU6050_initialize();           		//MPU6050��ʼ��
  DMP_Init();                     		//��ʼ��DMP
  RetargetInit(&huart3);				//ӳ��printf����������3�и�APPʹ��

  /* �������մ���3�����ж� */
  HAL_UART_Receive_IT(&huart3,Usart3_Receive_buf,sizeof(Usart3_Receive_buf));

  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //���������ⲿ�ж�

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  oled_show();
		if(Flag_Show==0)          		//ʹ��MiniBalance APP��OLED��ʾ��
		{
			 APP_Show();								//�������ݸ�APP
			 oled_show();          			//��ʾ����
		}
		else                      		//ʹ��MiniBalance��λ�� ��λ��ʹ�õ�ʱ����Ҫ�ϸ��ʱ�򣬹ʴ�ʱ�ر�app��ز��ֺ�OLED��ʾ��
		{
			 DataScope();          			//����MiniBalance��λ��
		}
		delay_flag=1;
		delay_50=0;
		while(delay_flag);	     			//ʾ������Ҫ50ms	�߾�����ʱ��delay����������Ҫ�󣬹�ʹ��MPU6050�ж��ṩ50ms��ʱ
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
