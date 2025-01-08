/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//�򻯱����Ķ���
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

//C���Կ����ͷ�ļ�
#include "stdio.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

//MPU6050DMP�����
#include <inv_mpu.h>
#include <inv_mpu_dmp_motion_driver.h>
#include <dmpKey.h>
#include <dmpmap.h>
#include <mpu6050.h>

//�弶֧�ְ����
#include "bsp_adc.h"
#include "delay.h"
#include "encoder.h"
#include "key.h"
#include "led.h"
#include "oled.h"
#include "usart3.h"
#include "iic.h"
#include "retarget.h"

//���ƺ������
#include <control.h>
#include <DataScope_DP.h>
#include <show.h>

//�ⲿ��������ʹ��
extern u8 Way_Angle;                             //��ȡ�Ƕȵ��㷨��1����Ԫ��  2��������  3�������˲�
extern u8 Flag_front,Flag_back,Flag_Left,Flag_Right,Flag_velocity; //����ң����صı���
extern u8 Flag_Stop,Flag_Show;       //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
extern int Encoder;            									 //���ұ��������������
extern int Motor,Servo;                           //���PWM����
extern int Velocity;                            	//��ʾ�¶�
extern int Voltage;
extern float balance_point;                                //��ص�ѹ������صı���
extern float phi,phi_dot,Gyro_Balance; //��� ��ǽ��ٶ� ƽ��������
extern float Show_Data_Mb;                         //ȫ����ʾ������������ʾ��Ҫ�鿴������
extern u8 delay_50,delay_flag,Flash_Send;//��ʱ�͵��εȱ���
extern u8 PID_Send;
extern u16 PID_Parameter[10],Flash_Parameter[10];  //Flash�������
extern int ki,kp,kd;
extern float balance_point_diff;
extern float BalancePoint_Offset;
extern float LeftControl;
extern float RightControl;
extern float Balance_Kp,Balance_Kd,Velocity_Kp,Velocity_Ki,Turn_Kp,Turn_Kd;//PID�������Ŵ�100����

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENCODER_B1_Pin GPIO_PIN_0
#define ENCODER_B1_GPIO_Port GPIOA
#define ENCODER_B2_Pin GPIO_PIN_1
#define ENCODER_B2_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOA
#define User_key_Pin GPIO_PIN_5
#define User_key_GPIO_Port GPIOA
#define ENCODER_A1_Pin GPIO_PIN_6
#define ENCODER_A1_GPIO_Port GPIOA
#define ENCODER_A2_Pin GPIO_PIN_7
#define ENCODER_A2_GPIO_Port GPIOA
#define MPU6050_INT_Pin GPIO_PIN_12
#define MPU6050_INT_GPIO_Port GPIOB
#define MPU6050_INT_EXTI_IRQn EXTI15_10_IRQn
#define IIC_SDA_Pin GPIO_PIN_15
#define IIC_SDA_GPIO_Port GPIOB
#define IIC_SCL_Pin GPIO_PIN_8
#define IIC_SCL_GPIO_Port GPIOA
#define SERVO_Pin GPIO_PIN_11
#define SERVO_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_15
#define OLED_DC_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_3
#define OLED_RES_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_4
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_5
#define OLED_SCL_GPIO_Port GPIOB
#define PWMA1_Pin GPIO_PIN_6
#define PWMA1_GPIO_Port GPIOB
#define PWMA2_Pin GPIO_PIN_7
#define PWMA2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//���ʹ��
#define SERVO  TIM1->CCR4

//����ʹ��
#define PWMA1  TIM4->CCR2  //PB6
#define PWMA2  TIM4->CCR1  //PB7

#define ADC_SampleTime_239Cycles5  ((uint8_t)0x07)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
